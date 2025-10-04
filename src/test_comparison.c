#include <check.h>

#include "s21_decimal.h"

START_TEST(test_is_equal_basic_positive) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(a, b), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_equal_basic_negative) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(a, b), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_equal_positive_negative_zeros) {
  s21_decimal pos_zero = {{0, 0, 0, 0}};
  s21_decimal neg_zero = {{0, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(pos_zero, neg_zero), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_equal_max_values) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal same_max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  ck_assert_int_eq(s21_is_equal(max_val, same_max), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_equal_negative_numbers) {
  s21_decimal a = {{5, 0, 0, 0x80000000}};
  s21_decimal b = {{5, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(a, b), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_not_equal_basic) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  ck_assert_int_eq(s21_is_not_equal(a, b), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_not_equal_same_values) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_not_equal(a, b), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_not_equal_different_signs) {
  s21_decimal positive = {{5, 0, 0, 0}};
  s21_decimal negative = {{5, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_not_equal(positive, negative), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_less_basic_positive) {
  s21_decimal smaller = {{3, 0, 0, 0}};
  s21_decimal larger = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(smaller, larger), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_less(larger, smaller), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_less_positive_vs_negative) {
  s21_decimal negative = {{5, 0, 0, 0x80000000}};
  s21_decimal positive = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(negative, positive), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_less(positive, negative), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_less_both_negative) {
  s21_decimal small_neg = {{3, 0, 0, 0x80000000}};
  s21_decimal large_neg = {{5, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less(large_neg, small_neg), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_less(small_neg, large_neg), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_less_edge_max_min) {
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  ck_assert_int_eq(s21_is_less(min_val, max_val), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_less_equal_values) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(a, b), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_less_or_equal_less) {
  s21_decimal smaller = {{3, 0, 0, 0}};
  s21_decimal larger = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(smaller, larger), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_less_or_equal_equal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_less_or_equal_greater) {
  s21_decimal smaller = {{3, 0, 0, 0}};
  s21_decimal larger = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(larger, smaller), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_less_or_equal_negative) {
  s21_decimal neg_small = {{3, 0, 0, 0x80000000}};
  s21_decimal neg_large = {{5, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less_or_equal(neg_large, neg_small),
                   S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_greater_basic) {
  s21_decimal smaller = {{3, 0, 0, 0}};
  s21_decimal larger = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(larger, smaller), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_greater(smaller, larger), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_greater_negative_vs_positive) {
  s21_decimal negative = {{5, 0, 0, 0x80000000}};
  s21_decimal positive = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(positive, negative), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_greater(negative, positive), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_greater_both_negative) {
  s21_decimal small_neg = {{3, 0, 0, 0x80000000}};
  s21_decimal large_neg = {{5, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_greater(small_neg, large_neg), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_greater(large_neg, small_neg), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_greater_equal_values) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_is_greater_or_equal_greater) {
  s21_decimal smaller = {{3, 0, 0, 0}};
  s21_decimal larger = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(larger, smaller),
                   S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_greater_or_equal_equal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_is_greater_or_equal_less) {
  s21_decimal smaller = {{3, 0, 0, 0}};
  s21_decimal larger = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(smaller, larger),
                   S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_edge_case_zero_vs_small_positive) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_decimal small = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(zero, small), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_greater(small, zero), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_edge_case_zero_vs_small_negative) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_decimal small_neg = {{1, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less(small_neg, zero), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_greater(zero, small_neg), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_edge_case_large_numbers) {
  s21_decimal almost_max = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  ck_assert_int_eq(s21_is_less(almost_max, max_val), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_greater(max_val, almost_max), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_edge_case_valid_decimals_only) {
  s21_decimal valid = {{5, 0, 0, 0}};

  ck_assert_int_eq(s21_is_equal(valid, valid), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_less(valid, valid), S21_COMPARISON_FALSE);
  ck_assert_int_eq(s21_is_greater(valid, valid), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_edge_case_different_signs_same_value) {
  s21_decimal positive = {{5, 0, 0, 0}};
  s21_decimal negative = {{5, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less(negative, positive), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_greater(positive, negative), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_equal(positive, negative), S21_COMPARISON_FALSE);
}
END_TEST

START_TEST(test_edge_case_very_large_numbers) {
  s21_decimal huge1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, 0}};
  s21_decimal huge2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  ck_assert_int_eq(s21_is_less(huge1, huge2), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_greater(huge2, huge1), S21_COMPARISON_TRUE);
}
END_TEST

START_TEST(test_edge_case_negative_zeros_comparison) {
  s21_decimal pos_zero = {{0, 0, 0, 0}};
  s21_decimal neg_zero = {{0, 0, 0, 0x80000000}};

  ck_assert_int_eq(s21_is_equal(pos_zero, neg_zero), S21_COMPARISON_TRUE);
  ck_assert_int_eq(s21_is_less(pos_zero, neg_zero), S21_COMPARISON_FALSE);
  ck_assert_int_eq(s21_is_greater(neg_zero, pos_zero), S21_COMPARISON_FALSE);
}
END_TEST

Suite *comparison_suite(void) {
  Suite *s;
  TCase *tc_core, *tc_edge;

  s = suite_create("s21_Comparison_tests");

  tc_core = tcase_create("Is_Equal_tests");
  tcase_add_test(tc_core, test_is_equal_basic_positive);
  tcase_add_test(tc_core, test_is_equal_basic_negative);
  tcase_add_test(tc_core, test_is_equal_positive_negative_zeros);
  tcase_add_test(tc_core, test_is_equal_max_values);
  tcase_add_test(tc_core, test_is_equal_negative_numbers);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("Is_Not_Equal_tests");
  tcase_add_test(tc_core, test_is_not_equal_basic);
  tcase_add_test(tc_core, test_is_not_equal_same_values);
  tcase_add_test(tc_core, test_is_not_equal_different_signs);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("Is_Less_tests");
  tcase_add_test(tc_core, test_is_less_basic_positive);
  tcase_add_test(tc_core, test_is_less_positive_vs_negative);
  tcase_add_test(tc_core, test_is_less_both_negative);
  tcase_add_test(tc_core, test_is_less_edge_max_min);
  tcase_add_test(tc_core, test_is_less_equal_values);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("Is_Les_Or_Equal_tests");
  tcase_add_test(tc_core, test_is_less_or_equal_less);
  tcase_add_test(tc_core, test_is_less_or_equal_equal);
  tcase_add_test(tc_core, test_is_less_or_equal_greater);
  tcase_add_test(tc_core, test_is_less_or_equal_negative);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("Is_Greater_tests");
  tcase_add_test(tc_core, test_is_greater_basic);
  tcase_add_test(tc_core, test_is_greater_negative_vs_positive);
  tcase_add_test(tc_core, test_is_greater_both_negative);
  tcase_add_test(tc_core, test_is_greater_equal_values);
  suite_add_tcase(s, tc_core);

  tc_core = tcase_create("Is_Greater_or_Equal_tests");
  tcase_add_test(tc_core, test_is_greater_or_equal_greater);
  tcase_add_test(tc_core, test_is_greater_or_equal_equal);
  tcase_add_test(tc_core, test_is_greater_or_equal_less);

  tc_edge = tcase_create("Edge_Cases_Comparison");
  tcase_add_test(tc_edge, test_edge_case_zero_vs_small_positive);
  tcase_add_test(tc_edge, test_edge_case_zero_vs_small_negative);
  tcase_add_test(tc_edge, test_edge_case_large_numbers);
  tcase_add_test(tc_edge, test_edge_case_valid_decimals_only);
  tcase_add_test(tc_edge, test_edge_case_different_signs_same_value);
  tcase_add_test(tc_edge, test_edge_case_very_large_numbers);
  tcase_add_test(tc_edge, test_edge_case_negative_zeros_comparison);
  suite_add_tcase(s, tc_core);
  suite_add_tcase(s, tc_edge);

  return s;
}
