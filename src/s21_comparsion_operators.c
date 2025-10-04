#include "s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int are_equal = S21_COMPARISON_FALSE;

  if (s21_is_decimal_invalid(value_1) || s21_is_decimal_invalid(value_2)) {
    are_equal = S21_COMPARISON_FALSE;
  } else if (s21_is_zero(value_1) && s21_is_zero(value_2)) {
    are_equal = S21_COMPARISON_TRUE;
  } else {
    s21_decimal a = value_1;
    s21_decimal b = value_2;
    s21_normalize(&a, &b);

    int sign_a = s21_get_sign(a);
    int sign_b = s21_get_sign(b);

    if (sign_a != sign_b) {
      are_equal = S21_COMPARISON_FALSE;
    } else {
      are_equal = (s21_compare_unsigned_big_int(a, b) == 0)
                      ? S21_COMPARISON_TRUE
                      : S21_COMPARISON_FALSE;
    }
  }

  return are_equal;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int is_less = S21_COMPARISON_FALSE;

  int is_invalid =
      s21_is_decimal_invalid(value_1) || s21_is_decimal_invalid(value_2);
  int are_both_zeros = s21_is_zero(value_1) && s21_is_zero(value_2);

  if (!is_invalid && !are_both_zeros) {
    s21_decimal a = value_1;
    s21_decimal b = value_2;
    s21_normalize(&a, &b);

    int sign_a = s21_get_sign(a);
    int sign_b = s21_get_sign(b);

    if (sign_a && !sign_b) {
      is_less = S21_COMPARISON_TRUE;
    } else if (!sign_a && sign_b) {
      is_less = S21_COMPARISON_FALSE;
    } else {
      int comparison = s21_compare_unsigned_big_int(a, b);

      if (sign_a && sign_b) {
        is_less = (comparison > 0) ? S21_COMPARISON_TRUE : S21_COMPARISON_FALSE;
      } else {
        is_less = (comparison < 0) ? S21_COMPARISON_TRUE : S21_COMPARISON_FALSE;
      }
    }
  }

  return is_less;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less_or_equal(value_1, value_2);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less(value_1, value_2);
}