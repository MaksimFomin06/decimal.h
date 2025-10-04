#include <check.h>

#include "s21_decimal.h"

START_TEST(test_get_set_sign) {
  s21_decimal dec = {0};

  s21_set_sign(&dec, 0);
  ck_assert_int_eq(s21_get_sign(dec), 0);

  s21_set_sign(&dec, 1);
  ck_assert_int_eq(s21_get_sign(dec), 1);

  s21_set_sign(NULL, 1);

  dec.bits[0] = 123;
  dec.bits[1] = 456;
  dec.bits[2] = 789;
  s21_set_sign(&dec, 1);
  ck_assert_int_eq(s21_get_sign(dec), 1);
  ck_assert_int_eq(dec.bits[0], 123);
  ck_assert_int_eq(dec.bits[1], 456);
  ck_assert_int_eq(dec.bits[2], 789);
}
END_TEST

START_TEST(test_get_set_scale) {
  s21_decimal dec = {0};

  for (int scale = 0; scale <= 28; scale++) {
    s21_set_scale(&dec, scale);
    ck_assert_int_eq(s21_get_scale(dec), scale);
  }

  s21_set_scale(&dec, 255);
  ck_assert_int_eq(s21_get_scale(dec), 255);

  s21_set_scale(&dec, 0);
  ck_assert_int_eq(s21_get_scale(dec), 0);

  dec.bits[0] = 123;
  dec.bits[1] = 456;
  dec.bits[2] = 789;
  s21_set_scale(&dec, 15);
  ck_assert_int_eq(s21_get_scale(dec), 15);
  ck_assert_int_eq(dec.bits[0], 123);
  ck_assert_int_eq(dec.bits[1], 456);
  ck_assert_int_eq(dec.bits[2], 789);
}
END_TEST

START_TEST(test_is_zero) {
  s21_decimal zero = {0};
  ck_assert_int_eq(s21_is_zero(zero), 1);

  s21_decimal non_zero = {0};
  non_zero.bits[0] = 1;
  ck_assert_int_eq(s21_is_zero(non_zero), 0);

  non_zero.bits[0] = 0;
  non_zero.bits[1] = 1;
  ck_assert_int_eq(s21_is_zero(non_zero), 0);

  non_zero.bits[1] = 0;
  non_zero.bits[2] = 1;
  ck_assert_int_eq(s21_is_zero(non_zero), 0);
}
END_TEST

START_TEST(test_clear) {
  s21_decimal dec = {0};
  dec.bits[0] = 123;
  dec.bits[1] = 456;
  dec.bits[2] = 789;
  dec.bits[3] = 0x80010000;

  s21_clear(&dec);

  ck_assert_int_eq(dec.bits[0], 0);
  ck_assert_int_eq(dec.bits[1], 0);
  ck_assert_int_eq(dec.bits[2], 0);
  ck_assert_int_eq(dec.bits[3], 0);

  s21_clear(NULL);
}
END_TEST

START_TEST(test_compare) {
  s21_decimal a = {0}, b = {0};

  a.bits[0] = 100;
  b.bits[0] = 100;
  ck_assert_int_eq(s21_compare(a, b), 0);

  a.bits[0] = 100;
  b.bits[0] = 200;
  ck_assert_int_eq(s21_compare(a, b), -1);

  a.bits[0] = 200;
  b.bits[0] = 100;
  ck_assert_int_eq(s21_compare(a, b), 1);

  a.bits[0] = 0;
  a.bits[1] = 100;
  b.bits[0] = 0;
  b.bits[1] = 200;
  ck_assert_int_eq(s21_compare(a, b), -1);

  a.bits[0] = 0;
  a.bits[1] = 200;
  b.bits[0] = 0;
  b.bits[1] = 100;
  ck_assert_int_eq(s21_compare(a, b), 1);

  a.bits[0] = 0;
  a.bits[1] = 0;
  a.bits[2] = 100;
  b.bits[0] = 0;
  b.bits[1] = 0;
  b.bits[2] = 200;
  ck_assert_int_eq(s21_compare(a, b), -1);

  a.bits[0] = 0;
  a.bits[1] = 0;
  a.bits[2] = 200;
  b.bits[0] = 0;
  b.bits[1] = 0;
  b.bits[2] = 100;
  ck_assert_int_eq(s21_compare(a, b), 1);
}
END_TEST

START_TEST(test_compare_absolute) {
  s21_decimal a = {0}, b = {0};

  a.bits[0] = 100;
  a.bits[3] = 0x80000000;
  b.bits[0] = 100;
  ck_assert_int_eq(s21_compare_absolute(a, b), 0);

  a.bits[0] = 100;
  a.bits[3] = 0x80000000;
  b.bits[0] = 200;
  ck_assert_int_eq(s21_compare_absolute(a, b), -1);

  a.bits[0] = 200;
  a.bits[3] = 0x80000000;
  b.bits[0] = 100;
  ck_assert_int_eq(s21_compare_absolute(a, b), 1);
}
END_TEST

START_TEST(test_multiply_by_10) {
  s21_decimal dec = {0};
  dec.bits[0] = 1;

  int result = s21_multiply_by_10(&dec);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dec.bits[0], 10);

  dec.bits[0] = UINT_MAX;
  result = s21_multiply_by_10(&dec);
  ck_assert_int_eq(result, 0);

  dec.bits[0] = UINT_MAX;
  dec.bits[1] = UINT_MAX;
  dec.bits[2] = UINT_MAX;
  result = s21_multiply_by_10(&dec);
  ck_assert_int_eq(result, 1);

  result = s21_multiply_by_10(NULL);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_divide_by_10) {
  s21_decimal dec = {0};
  dec.bits[0] = 100;
  int remainder = 0;

  int result = s21_divide_by_10(&dec, &remainder);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dec.bits[0], 10);
  ck_assert_int_eq(remainder, 0);

  dec.bits[0] = 123;
  result = s21_divide_by_10(&dec, &remainder);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dec.bits[0], 12);
  ck_assert_int_eq(remainder, 3);

  dec.bits[0] = 123;
  result = s21_divide_by_10(&dec, NULL);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dec.bits[0], 12);

  result = s21_divide_by_10(NULL, &remainder);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_shift_left) {
  s21_decimal dec = {0};
  dec.bits[0] = 1;

  int result = s21_shift_left(&dec);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dec.bits[0], 2);

  dec.bits[0] = 0x80000000;
  result = s21_shift_left(&dec);
  ck_assert_int_eq(result, 1);
  ck_assert_int_eq(dec.bits[0], 0);
  ck_assert_int_eq(dec.bits[1], 1);

  result = s21_shift_left(NULL);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_shift_right) {
  s21_decimal dec = {0};
  dec.bits[0] = 2;

  int result = s21_shift_right(&dec);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dec.bits[0], 1);

  dec.bits[0] = 1;
  result = s21_shift_right(&dec);
  ck_assert_int_eq(result, 1);
  ck_assert_int_eq(dec.bits[0], 0);

  result = s21_shift_right(NULL);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_normalize) {
  s21_decimal a = {0}, b = {0};

  a.bits[0] = 100;
  a.bits[3] = 0x00010000;
  b.bits[0] = 1000;
  b.bits[3] = 0x00020000;

  s21_normalize(&a, &b);

  ck_assert_int_eq(s21_get_scale(a), 2);
  ck_assert_int_eq(s21_get_scale(b), 2);
  ck_assert_int_eq(a.bits[0], 1000);
  ck_assert_int_eq(b.bits[0], 1000);

  s21_normalize(NULL, &b);
  s21_normalize(&a, NULL);

  a.bits[0] = 100;
  a.bits[3] = 0x00010000;
  b.bits[0] = 1000;
  b.bits[3] = 0x00010000;
  s21_normalize(&a, &b);
  ck_assert_int_eq(s21_get_scale(a), 1);
  ck_assert_int_eq(s21_get_scale(b), 1);
}
END_TEST

START_TEST(test_bank_round) {
  s21_decimal dec = {0};
  dec.bits[0] = 123;
  s21_set_scale(&dec, 2);
  s21_bank_round(&dec, 1);
  ck_assert_int_eq(dec.bits[0], 12);
  ck_assert_int_eq(s21_get_scale(dec), 1);

  s21_decimal dec2 = {0};
  dec2.bits[0] = 12345;
  s21_set_scale(&dec2, 3);
  s21_bank_round(&dec2, 2);
  ck_assert_int_eq(dec2.bits[0], 123);
  ck_assert_int_eq(s21_get_scale(dec2), 1);
}
END_TEST

START_TEST(test_is_decimal_invalid) {
  s21_decimal valid = {0};
  valid.bits[0] = 100;
  valid.bits[3] = 0x00010000;
  ck_assert_int_eq(s21_is_decimal_invalid(valid), 0);

  s21_decimal invalid_scale = {0};
  invalid_scale.bits[3] = 0x001D0000;
  ck_assert_int_eq(s21_is_decimal_invalid(invalid_scale), 1);

  s21_decimal invalid_bits = {0};
  invalid_bits.bits[3] = 0x7FFF0000;
  ck_assert_int_eq(s21_is_decimal_invalid(invalid_bits), 1);
}
END_TEST

START_TEST(test_copy_decimal) {
  s21_decimal src = {0};
  src.bits[0] = 123;
  src.bits[1] = 456;
  src.bits[2] = 789;
  src.bits[3] = 0x80010000;

  s21_decimal dst;
  s21_copy_decimal(src, &dst);

  ck_assert_int_eq(dst.bits[0], 123);
  ck_assert_int_eq(dst.bits[1], 456);
  ck_assert_int_eq(dst.bits[2], 789);
  ck_assert_uint_eq((unsigned int)dst.bits[3], 0x80010000);

  s21_copy_decimal(src, NULL);
}
END_TEST

START_TEST(test_compare_unsigned_big_int) {
  s21_decimal a = {0}, b = {0};

  a.bits[0] = 100;
  b.bits[0] = 100;
  ck_assert_int_eq(s21_compare_unsigned_big_int(a, b), 0);

  a.bits[0] = 100;
  b.bits[0] = 200;
  ck_assert_int_eq(s21_compare_unsigned_big_int(a, b), -1);

  a.bits[0] = 200;
  b.bits[0] = 100;
  ck_assert_int_eq(s21_compare_unsigned_big_int(a, b), 1);

  a.bits[0] = 0;
  a.bits[1] = 100;
  b.bits[0] = 0;
  b.bits[1] = 200;
  ck_assert_int_eq(s21_compare_unsigned_big_int(a, b), -1);

  a.bits[0] = 0;
  a.bits[1] = 200;
  b.bits[0] = 0;
  b.bits[1] = 100;
  ck_assert_int_eq(s21_compare_unsigned_big_int(a, b), 1);

  a.bits[0] = 0;
  a.bits[1] = 0;
  a.bits[2] = 100;
  b.bits[0] = 0;
  b.bits[1] = 0;
  b.bits[2] = 200;
  ck_assert_int_eq(s21_compare_unsigned_big_int(a, b), -1);

  a.bits[0] = 0;
  a.bits[1] = 0;
  a.bits[2] = 200;
  b.bits[0] = 0;
  b.bits[1] = 0;
  b.bits[2] = 100;
  ck_assert_int_eq(s21_compare_unsigned_big_int(a, b), 1);
}
END_TEST

START_TEST(test_get_set_exponent) {
  s21_decimal dec = {0};

  for (int exp = 0; exp <= 28; exp++) {
    s21_set_exponent(&dec, exp);
    ck_assert_int_eq(s21_get_exponent(dec), exp);
  }

  s21_set_exponent(&dec, 255);
  ck_assert_int_eq(s21_get_exponent(dec), 255);

  s21_set_exponent(&dec, 0);
  ck_assert_int_eq(s21_get_exponent(dec), 0);
}
END_TEST

START_TEST(test_init_decimal) {
  s21_decimal dec = {0};
  dec.bits[0] = 123;
  dec.bits[1] = 456;
  dec.bits[2] = 789;
  dec.bits[3] = 0x80010000;

  s21_init_decimal(&dec);

  ck_assert_int_eq(dec.bits[0], 0);
  ck_assert_int_eq(dec.bits[1], 0);
  ck_assert_int_eq(dec.bits[2], 0);
  ck_assert_int_eq(dec.bits[3], 0);

  s21_init_decimal(NULL);
}
END_TEST

Suite *help_functions(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_Help_tests");
  tc_core = tcase_create("Help_Core_tests");

  tcase_add_test(tc_core, test_get_set_sign);
  tcase_add_test(tc_core, test_get_set_scale);
  tcase_add_test(tc_core, test_is_zero);
  tcase_add_test(tc_core, test_clear);
  tcase_add_test(tc_core, test_compare);
  tcase_add_test(tc_core, test_compare_absolute);
  tcase_add_test(tc_core, test_multiply_by_10);
  tcase_add_test(tc_core, test_divide_by_10);
  tcase_add_test(tc_core, test_shift_left);
  tcase_add_test(tc_core, test_shift_right);
  tcase_add_test(tc_core, test_normalize);
  tcase_add_test(tc_core, test_bank_round);
  tcase_add_test(tc_core, test_is_decimal_invalid);
  tcase_add_test(tc_core, test_copy_decimal);
  tcase_add_test(tc_core, test_compare_unsigned_big_int);
  tcase_add_test(tc_core, test_get_set_exponent);
  tcase_add_test(tc_core, test_init_decimal);

  suite_add_tcase(s, tc_core);
  return s;
}
