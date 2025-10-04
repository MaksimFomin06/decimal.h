#include "s21_decimal.h"

int handle_addition(uint32_t* mntssA, uint32_t* mntssB, uint32_t* mntssR,
                    int* finalScale, int* finalSign, int sign1) {
  uint32_t extendedRes[4] = {0};
  uint64_t sumValue = (uint64_t)mntssA[0] + mntssB[0];
  extendedRes[0] = (uint32_t)sumValue;
  uint64_t carryOver = sumValue >> 32;

  sumValue = (uint64_t)mntssA[1] + mntssB[1] + carryOver;
  extendedRes[1] = (uint32_t)sumValue;
  carryOver = sumValue >> 32;

  sumValue = (uint64_t)mntssA[2] + mntssB[2] + carryOver;
  extendedRes[2] = (uint32_t)sumValue;
  carryOver = sumValue >> 32;
  extendedRes[3] = (uint32_t)carryOver;

  if (extendedRes[3] != 0u) {
    s21_reduce_wide(extendedRes, finalScale);
  }

  if (extendedRes[3] != 0u) {
    return (sign1 == 0) ? S21_INF_POS : S21_INF_NEG;
  }

  mntssR[0] = extendedRes[0];
  mntssR[1] = extendedRes[1];
  mntssR[2] = extendedRes[2];
  *finalSign = sign1;
  return S21_OK;
}

void handle_subtraction(uint32_t* mntssA, uint32_t* mntssB, uint32_t* mntssR,
                        int* finalSign, int sign1, int sign2) {
  int comparisonResult = s21_comp_(mntssA, mntssB);
  if (comparisonResult == 0) {
    // Результат 0, mntssR уже инициализирован нулями
    *finalSign = 0;
  } else if (comparisonResult > 0) {
    s21_sub_b96(mntssA, mntssB, mntssR);
    *finalSign = sign1;
  } else {
    s21_sub_b96(mntssB, mntssA, mntssR);
    *finalSign = sign2;
  }
}
int handle_division(uint32_t* dividendArr, uint32_t* divisorArr,
                    uint32_t* quotient, int* resultScale) {
  uint32_t remainder[3] = {0};

  // Битовая операция деления
  for (int bitIndex = 95; bitIndex >= 0; bitIndex--) {
    remainder[2] <<= 1;
    remainder[2] |= (remainder[1] >> 31);
    remainder[1] <<= 1;
    remainder[1] |= (remainder[0] >> 31);
    remainder[0] <<= 1;
    remainder[0] |= ((dividendArr[bitIndex / 32] >> (bitIndex % 32)) & 1u);

    if (s21_comp_(remainder, divisorArr) >= 0) {
      s21_sub_b96(remainder, divisorArr, remainder);
      quotient[bitIndex / 32] |= (1u << (bitIndex % 32));
    }
  }

  // Обработка остатка и масштабирование
  while (!s21_is_96_zero(remainder) && *resultScale < 28) {
    if (s21_mul_by_10_96(remainder)) {
      break;
    }
    (*resultScale)++;

    int digit = s21_div96_small(remainder, divisorArr);

    if (s21_mul_by_10_96(quotient)) {
      s21_sdob96(quotient);
      (*resultScale)--;
      break;
    }

    uint32_t addend[3] = {(uint32_t)digit, 0, 0};
    uint32_t carry = s21_add96(quotient, addend, quotient);
    if (carry) {
      s21_sdob96(quotient);
      (*resultScale)--;
      break;
    }
  }

  return S21_OK;
}

int check_overflow(uint32_t* quotient, int resultScale, int resultSign) {
  uint32_t max_decimal[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
  int comparison = s21_comp_(quotient, max_decimal);

  if (comparison > 0 || (comparison == 0 && resultScale == 0)) {
    return resultSign ? S21_INF_NEG : S21_INF_POS;
  } else if (resultScale > 28) {
    return resultSign ? S21_INF_NEG : S21_INF_POS;
  }
  return S21_OK;
}

void multiply_factors(s21_decimal factor1, s21_decimal factor2,
                      uint64_t* intermediate) {
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      uint64_t partialProduct =
          (uint64_t)factor1.bits[row] * (uint64_t)factor2.bits[col];
      intermediate[row + col] += partialProduct;
      int index = row + col;
      while (index < 5 && intermediate[index] > UINT32_MAX) {
        uint64_t carryPart = intermediate[index] >> 32;
        intermediate[index] &= UINT32_MAX;
        intermediate[index + 1] += carryPart;
        ++index;
      }
    }
  }
}

void normalize_intermediate(uint64_t* intermediate) {
  for (int idx = 0; idx < 5; ++idx) {
    if (intermediate[idx] > UINT32_MAX) {
      uint64_t carryPart = intermediate[idx] >> 32;
      intermediate[idx] &= UINT32_MAX;
      intermediate[idx + 1] += carryPart;
    }
  }
}

int scale_down_result(uint64_t* intermediate, int* totalScale) {
  while ((intermediate[5] != 0ull || intermediate[4] != 0ull ||
          intermediate[3] != 0ull) &&
         *totalScale > 0) {
    uint32_t adjustedIntermediate[6];
    for (int i = 0; i < 6; ++i) {
      adjustedIntermediate[i] = (uint32_t)intermediate[i];
    }
    s21_scale_down_one_banker_N(adjustedIntermediate, 6);
    for (int i = 0; i < 6; ++i) {
      intermediate[i] = (uint64_t)adjustedIntermediate[i];
    }
    --(*totalScale);
    normalize_intermediate(intermediate);
  }

  if (intermediate[5] != 0ull || intermediate[4] != 0ull ||
      intermediate[3] != 0ull) {
    return 1;  // Overflow
  }
  return 0;  // OK
}

int check_result_overflow(uint32_t* reducedResult, int finalSign) {
  uint32_t maxPossible[3] = {UINT32_MAX, UINT32_MAX, UINT32_MAX};
  if (s21_comp_(reducedResult, maxPossible) > 0) {
    return finalSign ? S21_INF_NEG : S21_INF_POS;
  }
  return S21_OK;
}

int s21_check_null_bits(s21_decimal d) {
  return (d.bits[0] | d.bits[1] | d.bits[2] | (d.bits[3] & 0x7F00FFFFu)) == 0;
}

u96 s21_u96_div_pow10(u96 m, int scale) {
  for (int i = 0; i < scale; ++i) {
    uint64_t remainder = 0;
    u96 result = {0};
    remainder = (remainder << 32) | m.bits[2];
    result.bits[2] = (uint32_t)(remainder / 10u);
    remainder %= 10u;
    remainder = (remainder << 32) | m.bits[1];
    result.bits[1] = (uint32_t)(remainder / 10u);
    remainder %= 10u;
    remainder = (remainder << 32) | m.bits[0];
    result.bits[0] = (uint32_t)(remainder / 10u);
    m = result;
  }
  return m;
}

int s21_is_96_zero(const uint32_t m[3]) { return (m[0] | m[1] | m[2]) == 0; }

int s21_comp_(const uint32_t a[3], const uint32_t b[3]) {
  if (a[2] != b[2]) return (a[2] < b[2]) ? -1 : 1;
  if (a[1] != b[1]) return (a[1] < b[1]) ? -1 : 1;
  if (a[0] != b[0]) return (a[0] < b[0]) ? -1 : 1;
  return 0;
}

uint32_t s21_add96(const uint32_t a[3], const uint32_t b[3], uint32_t out[3]) {
  uint32_t carry = 0u;
  uint64_t sum;

  sum = (uint64_t)a[0] + b[0] + carry;
  out[0] = (uint32_t)sum;
  carry = sum >> 32;

  sum = (uint64_t)a[1] + b[1] + carry;
  out[1] = (uint32_t)sum;
  carry = sum >> 32;

  sum = (uint64_t)a[2] + b[2] + carry;
  out[2] = (uint32_t)sum;
  carry = sum >> 32;

  return carry;
}

void s21_sub_b96(const uint32_t a[3], const uint32_t b[3], uint32_t out[3]) {
  uint64_t diff;
  uint64_t borrow = 0;

  diff = (uint64_t)a[0] - b[0] - borrow;
  out[0] = (uint32_t)diff;
  borrow = (diff >> 63) & 1u;

  diff = (uint64_t)a[1] - b[1] - borrow;
  out[1] = (uint32_t)diff;
  borrow = (diff >> 63) & 1u;

  diff = (uint64_t)a[2] - b[2] - borrow;
  out[2] = (uint32_t)diff;
}

int s21_mul_by_10_96(uint32_t m[3]) {
  int overflowFlag = 0;
  uint64_t product;

  product = (uint64_t)m[0] * 10u;
  m[0] = (uint32_t)product;
  uint64_t carry = product >> 32;

  product = (uint64_t)m[1] * 10u + carry;
  m[1] = (uint32_t)product;
  carry = product >> 32;

  product = (uint64_t)m[2] * 10u + carry;
  m[2] = (uint32_t)product;
  carry = product >> 32;

  overflowFlag = (carry != 0) ? 1 : 0;
  return overflowFlag;
}

uint32_t s21_divN_by_10(uint32_t* arr, int n) {
  uint32_t remainder = 0u;
  uint64_t value;

  for (int i = n - 1; i >= 0; --i) {
    value = ((uint64_t)remainder << 32) | (uint64_t)arr[i];
    uint32_t quotient = (uint32_t)(value / 10u);
    remainder = (uint32_t)(value % 10u);
    arr[i] = quotient;
  }
  return remainder;
}

uint32_t s21_addN_one(uint32_t* arr, int n) {
  uint32_t carryOut = 0u;
  uint64_t tempSum = (uint64_t)arr[0] + 1u;
  arr[0] = (uint32_t)tempSum;
  uint64_t carry = tempSum >> 32;

  for (int i = 1; i < n && carry; ++i) {
    tempSum = (uint64_t)arr[i] + (uint32_t)carry;
    arr[i] = (uint32_t)tempSum;
    carry = tempSum >> 32;
  }
  carryOut = (uint32_t)carry;
  return carryOut;
}

void s21_sdob96(uint32_t m[3]) {
  uint32_t wide[4] = {m[0], m[1], m[2], 0u};
  s21_scale_down_one_banker_N(wide, 4);
  m[0] = wide[0];
  m[1] = wide[1];
  m[2] = wide[2];
}

void s21_scale_down_one_banker_N(uint32_t* arr, int n) {
  uint32_t remainder = s21_divN_by_10(arr, n);

  if (remainder > 5u || (remainder == 5u && arr[n - 1] & 1u)) {
    s21_addN_one(arr, n);
  }
}

int divisible_by_10(const uint32_t m[3]) {
  uint32_t tempCopy[3] = {m[0], m[1], m[2]};
  return (s21_divN_by_10(tempCopy, 3) == 0u) ? 1 : 0;
}

void s21_trim_tz(uint32_t m[3], int* scale) {
  while (*scale > 0 && divisible_by_10(m)) {
    (void)s21_divN_by_10(m, 3);
    --(*scale);
  }
}

void s21_reduce_wide(uint32_t wide[4], int* scale) {
  while (wide[3] != 0u && *scale > 0) {
    s21_scale_down_one_banker_N(wide, 4);
    --(*scale);
  }
}

int s21_div96_small(uint32_t rem[3], const uint32_t divisor[3]) {
  int digits = 0;
  while (s21_comp_(rem, divisor) >= 0) {
    s21_sub_b96(rem, divisor, rem);
    digits++;
  }
  return digits;
}

int s21_get_scale_2(const s21_decimal dec) {
  return (int)((dec.bits[3] & SCALE_MASK) >> SCALE_SHIFT);
}

void s21_set_scale_2(s21_decimal* decimal, int newScale) {
  if (decimal) {
    decimal->bits[3] &= ~SCALE_MASK;
    decimal->bits[3] |= ((uint32_t)newScale << SCALE_SHIFT) & SCALE_MASK;
  }
}

int s21_get_sign_2(const s21_decimal dec) {
  return (int)((dec.bits[3] & SIGN_MASK) != 0);
}

void s21_set_sign_2(s21_decimal* decimal, int newSign) {
  if (decimal) {
    decimal->bits[3] &= ~SIGN_MASK;
    if (newSign) {
      decimal->bits[3] |= SIGN_MASK;
    }
  }
}

u96 s21_get_mantissa(const s21_decimal* d) {
  u96 mantissa = {{d->bits[0], d->bits[1], d->bits[2]}};
  return mantissa;
}

void s21_mantissa_init(u96* mantissa) { *mantissa = (u96){0}; }

void s21_decimal_init(s21_decimal* dec) { *dec = (s21_decimal){0}; }

void s21_bank_rounding(s21_decimal* decA, s21_decimal* decB) {
  int scaleA = s21_get_scale_2(*decA);
  int scaleB = s21_get_scale_2(*decB);
  if (scaleA == scaleB) return;
  uint32_t mantissaA[3] = {decA->bits[0], decA->bits[1], decA->bits[2]};
  uint32_t mantissaB[3] = {decB->bits[0], decB->bits[1], decB->bits[2]};

  while (scaleA != scaleB) {
    if (scaleA < scaleB) {
      uint32_t tempA[3] = {mantissaA[0], mantissaA[1], mantissaA[2]};
      if (!s21_mul_by_10_96(tempA)) {
        mantissaA[0] = tempA[0];
        mantissaA[1] = tempA[1];
        mantissaA[2] = tempA[2];
        ++scaleA;
      } else {
        uint32_t wideB[4] = {mantissaB[0], mantissaB[1], mantissaB[2], 0u};
        s21_scale_down_one_banker_N(wideB, 4);
        mantissaB[0] = wideB[0];
        mantissaB[1] = wideB[1];
        mantissaB[2] = wideB[2];
        --scaleB;
      }
    } else {
      uint32_t tempB[3] = {mantissaB[0], mantissaB[1], mantissaB[2]};
      if (!s21_mul_by_10_96(tempB)) {
        mantissaB[0] = tempB[0];
        mantissaB[1] = tempB[1];
        mantissaB[2] = tempB[2];
        ++scaleB;
      } else {
        uint32_t wideA[4] = {mantissaA[0], mantissaA[1], mantissaA[2], 0u};
        s21_scale_down_one_banker_N(wideA, 4);
        mantissaA[0] = wideA[0];
        mantissaA[1] = wideA[1];
        mantissaA[2] = wideA[2];
        --scaleA;
      }
    }
  }

  decA->bits[0] = mantissaA[0];
  decA->bits[1] = mantissaA[1];
  decA->bits[2] = mantissaA[2];
  s21_set_scale_2(decA, scaleA);

  decB->bits[0] = mantissaB[0];
  decB->bits[1] = mantissaB[1];
  decB->bits[2] = mantissaB[2];
  s21_set_scale_2(decB, scaleB);
}
