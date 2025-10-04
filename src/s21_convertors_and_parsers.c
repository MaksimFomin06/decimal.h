#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) return 1;

  for (int i = 0; i < 4; i++) dst->bits[i] = 0;

  if (src < 0) {
    dst->bits[3] |= (1u << 31);
    if (src != INT_MIN) {
      src = -src;
    } else {
      dst->bits[0] = 2147483648U;
      return 0;
    }
  }

  dst->bits[0] = src;
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (!dst) return 1;

  int sign = (src.bits[3] >> 31) & 1;
  int scale = (src.bits[3] >> 16) & 0xFF;

  unsigned int value = src.bits[0];

  for (int i = 0; i < scale; i++) {
    value /= 10;
  }

  if ((value > INT_MAX && !sign) ||
      (value > (unsigned int)INT_MAX + 1 && sign)) {
    return 1;
  }

  *dst = sign ? -(int)value : (int)value;
  return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (!dst) return 1;
  for (int i = 0; i < 4; i++) dst->bits[i] = 0;
  if (isnan(src) || isinf(src)) return 1;
  if (src == 0.0f) {
    if (signbit(src)) dst->bits[3] = (1u << 31);
    return 0;
  }
  int sign = signbit(src);
  double value = fabs(src);
  if (value < 1e-28 || value > 79228162514264337593543950335.0) return 1;

  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%.6e", value);
  value = atof(buffer);

  int scale = 0;
  while (scale < 28 && value < 1e14) {
    value *= 10.0;
    scale++;
  }
  uint64_t integer_part = (uint64_t)round(value);
  if (integer_part > 0xFFFFFFFFFFFFFFFF) return 1;
  dst->bits[0] = (uint32_t)integer_part;
  dst->bits[1] = (uint32_t)(integer_part >> 32);
  dst->bits[3] = (sign ? (1u << 31) : 0) | (scale << 16);
  return 0;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst) return 1;

  int sign = (src.bits[3] >> 31) & 1;
  int scale = (src.bits[3] >> 16) & 0xFF;

  double result = 0.0;
  result += (double)(unsigned int)src.bits[0];
  result += (double)(unsigned int)src.bits[1] * 4294967296.0;
  result += (double)(unsigned int)src.bits[2] * 4294967296.0 * 4294967296.0;

  for (int i = 0; i < scale; i++) {
    result /= 10.0;
  }

  *dst = (float)result;
  if (sign) *dst = -*dst;

  return 0;
}
