#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;

  *result = value;

  if (result->bits[3] & 0x80000000) {
    result->bits[3] &= 0x7FFFFFFF;
  } else {
    result->bits[3] |= 0x80000000;
  }

  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;

  result->bits[0] = 0;
  result->bits[1] = 0;
  result->bits[2] = 0;
  result->bits[3] = 0;

  int scale = (value.bits[3] >> 16) & 0xFF;
  int sign = (value.bits[3] >> 31) & 1;

  if (scale == 0) {
    *result = value;
    return 0;
  }

  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
    if (sign) {
      result->bits[3] = 0x80000000;
    }
    return 0;
  }

  s21_decimal temp = value;
  temp.bits[3] = 0;

  for (int i = 0; i < scale; i++) {
    unsigned int remainder = 0;

    for (int j = 2; j >= 0; j--) {
      uint64_t current =
          ((uint64_t)remainder << 32) | (unsigned int)temp.bits[j];
      temp.bits[j] = (unsigned int)(current / 10);
      remainder = (unsigned int)(current % 10);
    }
  }

  result->bits[0] = temp.bits[0];
  result->bits[1] = temp.bits[1];
  result->bits[2] = temp.bits[2];

  if (sign) {
    result->bits[3] = 0x80000000;
  }

  return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;

  result->bits[0] = 0;
  result->bits[1] = 0;
  result->bits[2] = 0;
  result->bits[3] = 0;

  int sign = (value.bits[3] >> 31) & 1;
  int scale = (value.bits[3] >> 16) & 0xFF;

  if (sign == 0 || scale == 0) {
    return s21_truncate(value, result);
  }

  s21_decimal truncated;
  truncated.bits[0] = 0;
  truncated.bits[1] = 0;
  truncated.bits[2] = 0;
  truncated.bits[3] = 0;

  if (s21_truncate(value, &truncated) != 0) {
    return 1;
  }

  int is_min_negative = ((unsigned int)truncated.bits[0] == 0xFFFFFFFF &&
                         (unsigned int)truncated.bits[1] == 0xFFFFFFFF &&
                         (unsigned int)truncated.bits[2] == 0xFFFFFFFF &&
                         (truncated.bits[3] & 0x7FFFFFFF) == 0);

  if (is_min_negative) {
    return 2;
  }

  s21_decimal one;
  one.bits[0] = 1;
  one.bits[1] = 0;
  one.bits[2] = 0;
  one.bits[3] = 0;

  return s21_sub(truncated, one, result);
}

int s21_round(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;
  s21_init_decimal(result);

  int scale = s21_get_scale(value), sign = s21_get_sign(value);
  if (scale == 0) {
    *result = value;
    return 0;
  }

  uint32_t num[3] = {(uint32_t)value.bits[0], (uint32_t)value.bits[1],
                     (uint32_t)value.bits[2]};
  int remainder = 0;

  for (int i = 0; i < scale; i++) {
    uint64_t carry = 0;
    for (int j = 2; j >= 0; j--) {
      uint64_t current = ((uint64_t)carry << 32) | num[j];
      num[j] = (uint32_t)(current / 10);
      carry = current % 10;
    }
    remainder = (int)carry;
  }

  if (remainder > 5 || (remainder == 5 && num[0] % 2 != 0)) {
    uint64_t sum = (uint64_t)num[0] + 1;
    num[0] = (uint32_t)sum;
    for (int i = 1; sum > 0xFFFFFFFF && i < 3; i++) {
      sum = (uint64_t)num[i] + 1;
      num[i] = (uint32_t)sum;
      if (i == 2 && sum > 0xFFFFFFFF) return 1;
    }
  }

  result->bits[0] = num[0];
  result->bits[1] = num[1];
  result->bits[2] = num[2];
  s21_set_scale(result, 0);
  s21_set_sign(result, sign);
  return 0;
}