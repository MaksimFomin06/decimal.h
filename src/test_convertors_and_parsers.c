
#include <check.h>

#include "s21_decimal.h"

START_TEST(test_int_to_decimal_positive) {
  s21_decimal result;
  int error = s21_from_int_to_decimal(12345, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_int_to_decimal_negative) {
  s21_decimal result;
  int error = s21_from_int_to_decimal(-67890, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 67890);
  ck_assert_int_eq((result.bits[3] >> 31) & 1, 1);
}
END_TEST

START_TEST(test_int_to_decimal_zero) {
  s21_decimal result;
  int error = s21_from_int_to_decimal(0, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_int_to_decimal_max_int) {
  s21_decimal result;
  int error = s21_from_int_to_decimal(INT_MAX, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], INT_MAX);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_int_to_decimal_min_int) {
  s21_decimal result;
  int error = s21_from_int_to_decimal(INT_MIN, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_uint_eq(result.bits[0] & 0xFFFFFFFF, 2147483648U);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 31) & 1, 1);
}
END_TEST

START_TEST(test_int_to_decimal_null_ptr) {
  int error = s21_from_int_to_decimal(123, NULL);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_decimal_to_int_positive) {
  s21_decimal src = {{12345, 0, 0, 0}};
  int result;
  int error = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, 12345);
}
END_TEST

START_TEST(test_decimal_to_int_negative) {
  s21_decimal src = {{67890, 0, 0, 1u << 31}};
  int result;
  int error = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, -67890);
}
END_TEST

START_TEST(test_decimal_to_int_zero) {
  s21_decimal src = {{0, 0, 0, 0}};
  int result;
  int error = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_decimal_to_int_with_scale) {
  s21_decimal src = {{123456, 0, 0, 2 << 16}};
  int result;
  int error = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, 1234);
}
END_TEST

START_TEST(test_decimal_to_int_overflow_positive) {
  s21_decimal src = {{0xFFFFFFFF, 0, 0, 0}};
  int result;
  int error = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_decimal_to_int_overflow_negative) {
  s21_decimal src = {{0xFFFFFFFF, 0, 0, 1u << 31}};
  int result;
  int error = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_decimal_to_int_large_with_scale) {
  s21_decimal src = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 28 << 16}};
  int result;
  int error = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(test_decimal_to_int_null_ptr) {
  s21_decimal src = {{123, 0, 0, 0}};
  int error = s21_from_decimal_to_int(src, NULL);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal) {
  s21_decimal dec;
  float num, check;

  num = 123.456f;
  ck_assert_int_eq(s21_from_float_to_decimal(num, &dec), 0);
  s21_from_decimal_to_float(dec, &check);
  ck_assert_float_eq_tol(num, check, 1e-6);

  num = -789.123f;
  ck_assert_int_eq(s21_from_float_to_decimal(num, &dec), 0);
  s21_from_decimal_to_float(dec, &check);
  ck_assert_float_eq_tol(num, check, 1e-6);

  num = 0.0f;
  ck_assert_int_eq(s21_from_float_to_decimal(num, &dec), 0);
  ck_assert_int_eq(dec.bits[0], 0);
  ck_assert_int_eq(dec.bits[3], 0);

  num = 7.9228162514264337593543950335e+28f;
  ck_assert_int_eq(s21_from_float_to_decimal(num, &dec), 0);

  num = -7.9228162514264337593543950335e+28f;
  ck_assert_int_eq(s21_from_float_to_decimal(num, &dec), 0);

  num = 1e-29f;
  ck_assert_int_eq(s21_from_float_to_decimal(num, &dec), 1);

  num = 8e+28f;
  ck_assert_int_eq(s21_from_float_to_decimal(num, &dec), 1);

  num = NAN;
  ck_assert_int_eq(s21_from_float_to_decimal(num, &dec), 1);

  num = INFINITY;
  ck_assert_int_eq(s21_from_float_to_decimal(num, &dec), 1);
}
END_TEST

START_TEST(test_decimal_to_float_positive) {
  s21_decimal src = {{123456, 0, 0, 2 << 16}};
  float result;
  int error = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(error, 0);
  ck_assert(fabsf(result - 1234.56f) < 0.001f);
}
END_TEST

START_TEST(test_decimal_to_float_negative) {
  s21_decimal src = {{7890, 0, 0, (1u << 31) | (2 << 16)}};
  float result;
  int error = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(error, 0);
  ck_assert(fabsf(result + 78.90f) < 0.1f);
}
END_TEST
START_TEST(test_decimal_to_float_zero) {
  s21_decimal src = {{0, 0, 0, 0}};  // 0
  float result;
  int error = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(error, 0);
  ck_assert(fabsf(result) < 0.000001f);
}
END_TEST

START_TEST(test_decimal_to_float_large) {
  s21_decimal src = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  float result;
  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert(fabsf(result) > 1e10f);
}
END_TEST

START_TEST(test_decimal_to_float_small) {
  s21_decimal src = {{1, 0, 0, 28 << 16}};
  float result;
  int error = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(error, 0);
  ck_assert(fabsf(result - 1e-28f) < 1e-30f);
}
END_TEST

START_TEST(test_decimal_to_float_null_ptr) {
  s21_decimal src = {{123, 0, 0, 0}};
  int error = s21_from_decimal_to_float(src, NULL);
  ck_assert_int_eq(error, 1);
}
END_TEST

Suite *conversion_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_Conversion_tests");

  tc_core = tcase_create("Int_To_Decimal_tests");
  tcase_add_test(tc_core, test_int_to_decimal_positive);
  tcase_add_test(tc_core, test_int_to_decimal_negative);
  tcase_add_test(tc_core, test_int_to_decimal_zero);
  tcase_add_test(tc_core, test_int_to_decimal_max_int);
  tcase_add_test(tc_core, test_int_to_decimal_min_int);
  tcase_add_test(tc_core, test_int_to_decimal_null_ptr);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("Decimal_To_Int_tests");
  tcase_add_test(tc_core, test_decimal_to_int_positive);
  tcase_add_test(tc_core, test_decimal_to_int_negative);
  tcase_add_test(tc_core, test_decimal_to_int_zero);
  tcase_add_test(tc_core, test_decimal_to_int_with_scale);
  tcase_add_test(tc_core, test_decimal_to_int_overflow_positive);
  tcase_add_test(tc_core, test_decimal_to_int_overflow_negative);
  tcase_add_test(tc_core, test_decimal_to_int_large_with_scale);
  tcase_add_test(tc_core, test_decimal_to_int_null_ptr);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("Float_To_Decimal_tests");
  tcase_add_test(tc_core, test_s21_from_float_to_decimal);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("Decimal_To_Float_tests");
  tcase_add_test(tc_core, test_decimal_to_float_positive);
  tcase_add_test(tc_core, test_decimal_to_float_negative);
  tcase_add_test(tc_core, test_decimal_to_float_zero);
  tcase_add_test(tc_core, test_decimal_to_float_large);
  tcase_add_test(tc_core, test_decimal_to_float_small);
  tcase_add_test(tc_core, test_decimal_to_float_null_ptr);
  suite_add_tcase(s, tc_core);

  return s;
}
