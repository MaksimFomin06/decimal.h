#include "s21_decimal.h"

int s21_get_sign(s21_decimal num) { return (num.bits[3] >> 31) & 1; }

void s21_set_sign(s21_decimal *value, int sign) {
  if (value) {
    int scale = s21_get_scale(*value);
    if (sign) {
      value->bits[3] = (1u << 31) | (scale << 16);
    } else {
      value->bits[3] = (scale << 16);
    }
  }
}

int s21_get_scale(s21_decimal num) {
  int scale = (num.bits[3] & 0x00FF0000) >> 16;
  return scale;
}

void s21_set_scale(s21_decimal *num, int scale) {
  num->bits[3] &= ~(0xFF << 16);
  num->bits[3] |= (scale & 0xFF) << 16;
}

int s21_is_zero(s21_decimal num) {
  return num.bits[0] == 0 && num.bits[1] == 0 && num.bits[2] == 0;
}

void s21_clear(s21_decimal *num) {
  if (num != NULL) {
    memset(num, 0, sizeof(s21_decimal));
  }
}

int s21_compare(s21_decimal a, s21_decimal b) {
  for (int i = 2; i >= 0; i--) {
    if (a.bits[i] > b.bits[i]) return 1;
    if (a.bits[i] < b.bits[i]) return -1;
  }
  return 0;
}
int s21_compare_absolute(s21_decimal a, s21_decimal b) {
  for (int i = 2; i >= 0; i--) {
    if ((uint32_t)a.bits[i] > (uint32_t)b.bits[i]) {
      return 1;
    }
    if ((uint32_t)a.bits[i] < (uint32_t)b.bits[i]) {
      return -1;
    }
  }
  return 0;
}

int s21_multiply_by_10(s21_decimal *value) {
  if (value == NULL) return 1;

  s21_decimal max_div_10 = {{0x19999999, 0x99999999, 0x19999999, 0}};
  if (s21_compare_absolute(*value, max_div_10) > 0) {
    return 1;
  }

  uint64_t carry = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t product = (uint64_t)value->bits[i] * 10 + carry;
    value->bits[i] = product & 0xFFFFFFFF;
    carry = product >> 32;
  }

  if (carry > 0) {
    return 1;
  }

  return 0;
}

int s21_divide_by_10(s21_decimal *value, int *remainder) {
  if (!value) return 1;
  uint64_t carry = 0;
  for (int i = 2; i >= 0; i--) {
    uint64_t temp = (carry << 32) | (uint64_t)value->bits[i];
    value->bits[i] = (uint32_t)(temp / 10);
    carry = temp % 10;
  }
  if (remainder) *remainder = (int)carry;
  return 0;
}

int s21_shift_left(s21_decimal *num) {
  if (num == NULL) return 1;
  int any_carry = 0;
  unsigned int carry = 0;
  for (int i = 0; i < 3; i++) {
    unsigned int bits_as_unsigned = *(unsigned int *)&num->bits[i];
    unsigned long long value = (unsigned long long)bits_as_unsigned << 1;
    unsigned int result_bits = (unsigned int)(value & 0xFFFFFFFF) | carry;
    num->bits[i] = *(int *)&result_bits;
    carry = (unsigned int)(value >> 32);
    if (carry) {
      any_carry = 1;
    }
  }
  return any_carry;
}

int s21_shift_right(s21_decimal *num) {
  if (num == NULL) return 1;
  int any_lost = 0;
  unsigned int carry = 0;
  for (int i = 2; i >= 0; i--) {
    unsigned int bits_as_unsigned = *(unsigned int *)&num->bits[i];
    any_lost |= (bits_as_unsigned & 1);
    unsigned int new_value = (bits_as_unsigned >> 1) | carry;
    num->bits[i] = *(int *)&new_value;
    carry = (bits_as_unsigned & 1) ? 0x80000000 : 0;
  }
  return any_lost;
}

void s21_normalize(s21_decimal *value1, s21_decimal *value2) {
  if (!value1 || !value2) return;
  int scale1 = s21_get_scale(*value1);
  int scale2 = s21_get_scale(*value2);
  if (scale1 == scale2) return;
  if (scale1 < scale2) {
    int can_scale_up = 1;
    s21_decimal temp = *value1;

    for (int i = scale1; i < scale2; i++) {
      if (s21_multiply_by_10(&temp)) {
        can_scale_up = 0;
        break;
      }
    }

    if (can_scale_up) {
      *value1 = temp;
      s21_set_scale(value1, scale2);
    } else {
      s21_bank_round(value2, scale2 - scale1);
    }
  } else {
    int can_scale_up = 1;
    s21_decimal temp = *value2;

    for (int i = scale2; i < scale1; i++) {
      if (s21_multiply_by_10(&temp)) {
        can_scale_up = 0;
        break;
      }
    }

    if (can_scale_up) {
      *value2 = temp;
      s21_set_scale(value2, scale1);
    } else {
      s21_bank_round(value1, scale1 - scale2);
    }
  }
}

int s21_bank_round(s21_decimal *num, int reduce_by) {
  if (num == NULL || reduce_by <= 0) return 1;
  int initial_scale = s21_get_scale(*num);
  int sign = s21_get_sign(*num);
  for (int i = 0; i < reduce_by; i++) {
    if (s21_is_zero(*num)) break;
    unsigned int remainder = 0;
    s21_decimal temp = {0};
    for (int j = 2; j >= 0; j--) {
      unsigned long long value =
          ((unsigned long long)remainder << 32) | (unsigned int)num->bits[j];
      temp.bits[j] = (int)(value / 10);
      remainder = (unsigned int)(value % 10);
    }
    if (remainder > 5 || (remainder == 5 && (temp.bits[0] & 1))) {
      unsigned int carry = 1;
      for (int k = 0; k < 3 && carry; k++) {
        unsigned long long sum = (unsigned long long)temp.bits[k] + carry;
        temp.bits[k] = (int)(sum & 0xFFFFFFFF);
        carry = (sum > 0xFFFFFFFF) ? 1 : 0;
      }
    }
    for (int k = 0; k < 4; k++) {
      num->bits[k] = temp.bits[k];
    }
  }
  int new_scale = initial_scale - reduce_by;
  if (new_scale < 0) new_scale = 0;
  s21_set_scale(num, new_scale);
  if (s21_is_zero(*num)) {
    s21_set_sign(num, 0);
  } else {
    s21_set_sign(num, sign);
  }

  return 0;
}

int s21_is_decimal_invalid(s21_decimal dec) {
  if ((dec.bits[3] & 0x7F00FFFF) != 0) return 1;
  if (s21_get_exponent(dec) > 28) return 1;
  return 0;
}

void s21_copy_decimal(s21_decimal src, s21_decimal *dst) {
  if (dst) {
    for (int i = 0; i < 4; i++) {
      dst->bits[i] = src.bits[i];
    }
  }
}

int s21_compare_unsigned_big_int(s21_decimal a, s21_decimal b) {
  if (a.bits[2] > b.bits[2]) return 1;
  if (a.bits[2] < b.bits[2]) return -1;

  if (a.bits[1] > b.bits[1]) return 1;
  if (a.bits[1] < b.bits[1]) return -1;

  if (a.bits[0] > b.bits[0]) return 1;
  if (a.bits[0] < b.bits[0]) return -1;

  return 0;
}

int s21_get_exponent(s21_decimal decimal) {
  return (decimal.bits[3] >> 16) & 0xFF;
}

void s21_set_exponent(s21_decimal *decimal, int exponent) {
  decimal->bits[3] &= ~(0xFF << 16);
  decimal->bits[3] |= (exponent & 0xFF) << 16;
}

void s21_init_decimal(s21_decimal *decimal) {
  if (decimal != NULL) {
    for (int i = 0; i < 4; i++) {
      decimal->bits[i] = 0;
    }
  }
}