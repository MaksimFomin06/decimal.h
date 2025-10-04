#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DECIMAL 79228162514264337593543950335.0
#define MIN_DECIMAL 1e-28
#define S21_DECIMAL_OK 0
#define S21_DECIMAL_ERROR_CONVERSION 1
#define S21_DECIMAL_ERROR_CALCULATION 2
#define S21_DECIMAL_ERROR_BIG 1
#define S21_DECIMAL_ERROR_SMALL 2
#define S21_DECIMAL_ERROR_ZERO_DIV 3
#define S21_COMPARISON_FALSE 0
#define S21_COMPARISON_TRUE 1
#define S21_TRUE 1
#define S21_FALSE 0
#define DECIMAL_MIN 1e-28f
#define DECIMAL_MAX 1e+28f
#define S21_ERROR
#define SIGN_MASK 0x80000000u
#define SCALE_MASK 0x00FF0000u
#define SCALE_SHIFT 16

typedef struct {
  int bits[4];
} s21_decimal;

enum {
  S21_OK = 0,
  S21_INF_POS = 1,
  S21_INF_NEG = 2,
  S21_DIV_ZERO = 3,
  S21_CONV_ERR = 1,
  S21_CALC_ERR = 1,
};

typedef struct u96 {
  uint32_t bits[3];
} u96;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

int handle_addition(uint32_t *mntssA, uint32_t *mntssB, uint32_t *mntssR,
                    int *finalScale, int *finalSign, int sign1);
void handle_subtraction(uint32_t *mntssA, uint32_t *mntssB, uint32_t *mntssR,
                        int *finalSign, int sign1, int sign2);
int handle_division(uint32_t *dividendArr, uint32_t *divisorArr,
                    uint32_t *quotient, int *resultScale);
int check_overflow(uint32_t *quotient, int resultScale, int resultSign);
void multiply_factors(s21_decimal factor1, s21_decimal factor2,
                      uint64_t *intermediate);
void normalize_intermediate(uint64_t *intermediate);
int check_result_overflow(uint32_t *reducedResult, int finalSign);
int scale_down_result(uint64_t *intermediate, int *totalScale);

int s21_is_max(s21_decimal value);
int s21_get_sign(s21_decimal num);
void s21_set_sign(s21_decimal *num, int sign);
int s21_get_scale(s21_decimal num);
void s21_set_scale(s21_decimal *num, int scale);
int s21_is_zero(s21_decimal num);
int s21_is_min_value(s21_decimal value);
void s21_clear(s21_decimal *num);
void s21_copy_decimal(s21_decimal src, s21_decimal *dst);
int s21_compare(s21_decimal a, s21_decimal b);
int s21_compare_absolute(s21_decimal a, s21_decimal b);
int s21_compare_unsigned_big_int(s21_decimal a, s21_decimal b);
int s21_is_decimal_invalid(s21_decimal dec);
int s21_get_exponent(s21_decimal decimal);
void s21_set_exponent(s21_decimal *decimal, int exponent);
int s21_multiply_by_10(s21_decimal *value);
int s21_divide_by_10(s21_decimal *value, int *remainder);
int s21_shift_left(s21_decimal *num);
int s21_shift_right(s21_decimal *num);
void s21_normalize(s21_decimal *value1, s21_decimal *value2);
int s21_bank_round(s21_decimal *num, int reduce_by);
void s21_init_decimal(s21_decimal *decimal);
int s21_div_fractional_case(uint64_t dividend, uint64_t divisor,
                            s21_decimal *result, int final_sign,
                            int final_scale);
int s21_div_simple_case(uint64_t dividend, uint64_t divisor,
                        s21_decimal *result, int final_sign, int final_scale);
void s21_multiply_parts(s21_decimal value_1, s21_decimal value_2,
                        uint64_t temp[6]);
int s21_check_mul_overflow(uint64_t temp[6]);

void s21_mantissa_init(u96 *mantissa);
int s21_check_null_bits(s21_decimal d);
u96 s21_u96_div_pow10(u96 m, int scale);
u96 s21_get_mantissa(const s21_decimal *d);
void s21_decimal_init(s21_decimal *dec);

void s21_reduce_wide(uint32_t wide[4], int *scale);
int s21_mul_by_10_96(uint32_t m[3]);

int s21_get_sign_2(const s21_decimal dec);
int s21_get_scale_2(const s21_decimal dec);
void s21_set_sign_2(s21_decimal *dec, int sign);
void s21_set_scale_2(s21_decimal *dec, int s);

int s21_comp_(const uint32_t a[3], const uint32_t b[3]);
void s21_sub_b96(const uint32_t a[3], const uint32_t b[3], uint32_t out[3]);
void s21_bank_rounding(s21_decimal *dec_a, s21_decimal *dec_b);
void s21_scale_down_one_banker_N(uint32_t *arr, int n);
void s21_sdob96(uint32_t m[3]);
void s21_trim_tz(uint32_t m[3], int *scale);

int s21_is_96_zero(const uint32_t m[3]);

uint32_t s21_add96(const uint32_t a[3], const uint32_t b[3], uint32_t out[3]);
int s21_div96_small(uint32_t rem[3], const uint32_t mb[3]);

uint32_t s21_divN_by_10(uint32_t *arr, int n);
uint32_t s21_addN_one(uint32_t *arr, int n);

#endif