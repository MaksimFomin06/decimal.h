#include <check.h>

#include "s21_decimal.h"

START_TEST(test_round_positive) {
  s21_decimal value = {{12355, 0, 0, 2 << 16}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_round(value, &result));
  ck_assert_int_eq(124, result.bits[0]);
  ck_assert_int_eq(0, result.bits[3]);
}
END_TEST

START_TEST(test_round_negative) {
  s21_decimal value = {{98765, 0, 0, (3 << 16) | 0x80000000}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_round(value, &result));
  ck_assert_int_eq(99, result.bits[0]);
  ck_assert_uint_eq(0x80000000, (unsigned int)result.bits[3]);
}
END_TEST

START_TEST(test_round_bankers_even) {
  s21_decimal value = {{5, 0, 0, 1 << 16}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_round(value, &result));
  ck_assert_int_eq(0, result.bits[0]);
}
END_TEST

START_TEST(test_round_bankers_odd) {
  s21_decimal value = {{15, 0, 0, 1 << 16}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_round(value, &result));
  ck_assert_int_eq(2, result.bits[0]);
}
END_TEST

START_TEST(test_round_max_scale) {
  s21_decimal value = {{123456789, 0, 0, 28 << 16}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_round(value, &result));
  ck_assert_int_eq(0, result.bits[0]);  // → 0
}
END_TEST

START_TEST(test_round_overflow) {
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1 << 16}};
  s21_decimal result;

  int status = s21_round(value, &result);
  ck_assert(status == 0 || status == 1);

  if (status == 0) {
    ck_assert_uint_eq(0, (unsigned int)result.bits[3] & 0x7FFFFFFF);
  }
}
END_TEST

START_TEST(test_truncate_positive) {
  s21_decimal value = {{12399, 0, 0, 2 << 16}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_truncate(value, &result));
  ck_assert_int_eq(123, result.bits[0]);
}
END_TEST

START_TEST(test_truncate_negative) {
  s21_decimal value = {{12399, 0, 0, (2 << 16) | 0x80000000}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_truncate(value, &result));
  ck_assert_int_eq(123, result.bits[0]);
  ck_assert_uint_eq(0x80000000, (unsigned int)result.bits[3]);
}
END_TEST

START_TEST(test_truncate_zero_scale) {
  s21_decimal value = {{123, 0, 0, 0}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_truncate(value, &result));
  ck_assert_int_eq(123, result.bits[0]);
}
END_TEST

START_TEST(test_truncate_small_fraction) {
  s21_decimal value = {{1, 0, 0, 28 << 16}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_truncate(value, &result));
  ck_assert_int_eq(0, result.bits[0]);
}
END_TEST

START_TEST(test_floor_positive) {
  s21_decimal value = {{12399, 0, 0, 2 << 16}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_floor(value, &result));
  ck_assert_int_eq(123, result.bits[0]);
}
END_TEST

START_TEST(test_floor_negative) {
  s21_decimal value = {{12399, 0, 0, (2 << 16) | 0x80000000}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_floor(value, &result));
  ck_assert_int_eq(124, result.bits[0]);
  ck_assert_uint_eq(0x80000000, (unsigned int)result.bits[3]);
}
END_TEST

START_TEST(test_floor_min_negative) {
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_floor(value, &result));

  unsigned int temp = result.bits[0];
  ck_assert_uint_eq(0xFFFFFFFF, temp);

  temp = result.bits[1];
  ck_assert_uint_eq(0xFFFFFFFF, temp);

  temp = result.bits[2];
  ck_assert_uint_eq(0xFFFFFFFF, temp);

  temp = result.bits[3];
  ck_assert_uint_eq(0x80000000, temp);
}
END_TEST

START_TEST(test_negate_positive) {
  s21_decimal value = {{123, 0, 0, 2 << 16}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_negate(value, &result));
  ck_assert_int_eq(123, result.bits[0]);
  ck_assert_uint_eq((2 << 16) | 0x80000000, (unsigned int)result.bits[3]);
}
END_TEST

START_TEST(test_negate_negative) {
  s21_decimal value = {{123, 0, 0, (2 << 16) | 0x80000000}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_negate(value, &result));
  ck_assert_int_eq(123, result.bits[0]);
  ck_assert_uint_eq(2 << 16, (unsigned int)result.bits[3]);
}
END_TEST

START_TEST(test_negate_zero) {
  s21_decimal value = {{0, 0, 0, (3 << 16) | 0x80000000}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_negate(value, &result));
  ck_assert_int_eq(0, result.bits[0]);
  ck_assert_int_eq(3 << 16, result.bits[3]);
}
END_TEST

START_TEST(test_negate_max_value) {
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal result;
  int ret = s21_negate(value, &result);
  ck_assert_int_eq(0, ret);
  unsigned int temp0 = result.bits[0];
  unsigned int temp3 = result.bits[3];
  ck_assert_uint_eq(0xFFFFFFFF, temp0);
  ck_assert_uint_eq(0x80000000, temp3);
}
END_TEST

void print_decimal(const char *name, s21_decimal d) {
  printf("%s: [%u, %u, %u, %u]\n", name, d.bits[0], d.bits[1], d.bits[2],
         d.bits[3]);
}

int is_equal_decimal(s21_decimal a, s21_decimal b) {
  return a.bits[0] == b.bits[0] && a.bits[1] == b.bits[1] &&
         a.bits[2] == b.bits[2] && a.bits[3] == b.bits[3];
}

START_TEST(test_round_bankers_all_cases) {
  for (int i = 0; i <= 9; i++) {
    s21_decimal value = {{i, 0, 0, 1 << 16}};
    s21_decimal result;

    ck_assert_int_eq(0, s21_round(value, &result));

    int expected = (i == 5) ? 0 : (i >= 5) ? 1 : 0;
    ck_assert_int_eq(expected, result.bits[0]);
  }
}
END_TEST

START_TEST(test_round_large_numbers) {
  s21_decimal test_cases[] = {{{0xFFFFFFFA, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
                              {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
                              {{123456789, 123456789, 123456789, 5 << 16}}};

  for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
    s21_decimal result;
    int status = s21_round(test_cases[i], &result);
    ck_assert(status == 0 || status == 1);
  }
}
END_TEST

START_TEST(test_round_precision_loss) {
  s21_decimal value = {{1, 0, 0, 28 << 16}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_round(value, &result));
  ck_assert_int_eq(0, result.bits[0]);
}
END_TEST

START_TEST(test_truncate_extreme_values) {
  typedef struct {
    s21_decimal input;
    unsigned int expected;
    int scale;
  } TestCase;

  TestCase cases[] = {
      {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}}, 0xFFFFFFFF, 0},
      {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1 << 16}}, 0x99999999, 1},
      {{{123456789, 0, 0, 8 << 16}}, 1, 8},
  };

  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
    s21_decimal result;
    ck_assert_int_eq(0, s21_truncate(cases[i].input, &result));
    ck_assert_uint_eq(cases[i].expected, (unsigned int)result.bits[0]);
  }
}
END_TEST

START_TEST(test_truncate_negative_zero) {
  s21_decimal neg_zero = {{0, 0, 0, 0x80000000}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_truncate(neg_zero, &result));
  ck_assert_int_eq(0, result.bits[0]);
  ck_assert_uint_eq(0x80000000, (unsigned int)result.bits[3]);
}
END_TEST

START_TEST(test_floor_negative_precision) {
  s21_decimal cases[] = {
      {{99, 0, 0, (2 << 16) | 0x80000000}},
      {{50, 0, 0, (2 << 16) | 0x80000000}},
      {{1, 0, 0, (2 << 16) | 0x80000000}},
  };

  unsigned int expected[] = {1, 1, 1};

  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
    s21_decimal result;
    ck_assert_int_eq(0, s21_floor(cases[i], &result));
    ck_assert_uint_eq(expected[i], (unsigned int)result.bits[0]);
    ck_assert_uint_eq(0x80000000, (unsigned int)result.bits[3]);
  }
}
END_TEST

START_TEST(test_floor_overflow_protection) {
  s21_decimal value = {{0x99999999, 0x99999999, 0x19999999, 0x80010000}};
  s21_decimal result;

  ck_assert_int_eq(0, s21_floor(value, &result));
}
END_TEST

START_TEST(test_negate_preserves_scale) {
  for (int scale = 0; scale <= 28; scale++) {
    s21_decimal value = {{123, 0, 0, scale << 16}};
    s21_decimal result;

    ck_assert_int_eq(0, s21_negate(value, &result));
    ck_assert_int_eq(123, result.bits[0]);
    ck_assert_uint_eq((scale << 16) | 0x80000000, (unsigned int)result.bits[3]);
  }
}
END_TEST

START_TEST(test_negate_double_negate) {
  s21_decimal original = {{12345, 0, 0, 3 << 16}};
  s21_decimal temp, result;

  ck_assert_int_eq(0, s21_negate(original, &temp));
  ck_assert_int_eq(0, s21_negate(temp, &result));

  ck_assert_int_eq(original.bits[0], result.bits[0]);
  ck_assert_int_eq(original.bits[1], result.bits[1]);
  ck_assert_int_eq(original.bits[2], result.bits[2]);
  ck_assert_int_eq(original.bits[3], result.bits[3]);
}
END_TEST

START_TEST(test_stress_random) {
  s21_decimal values[] = {
      {{0, 0, 0, 0}},          {{1, 0, 0, 0}},
      {{0xFFFFFFFF, 0, 0, 0}}, {{0, 0xFFFFFFFF, 0, 0}},
      {{0, 0, 0xFFFFFFFF, 0}}, {{0xAAAAAAAA, 0x55555555, 0x33333333, 0}},
  };

  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
    s21_decimal result;

    ck_assert(s21_round(values[i], &result) == 0 ||
              s21_round(values[i], &result) == 1);
    ck_assert(s21_truncate(values[i], &result) == 0);
    ck_assert(s21_floor(values[i], &result) == 0 ||
              s21_floor(values[i], &result) == 2);
    ck_assert(s21_negate(values[i], &result) == 0);
  }
}
END_TEST

START_TEST(test_invalid_decimal_state) {
  s21_decimal invalid_cases[] = {
      {{0, 0, 0, 0xFFFFFFFF}},
      {{0, 0, 0, 0x7FFFFFFF}},
      {{0, 0, 0, 0x80000000}},
  };

  for (size_t i = 0; i < sizeof(invalid_cases) / sizeof(invalid_cases[0]);
       i++) {
    s21_decimal result;
    int round_status = s21_round(invalid_cases[i], &result);
    int truncate_status = s21_truncate(invalid_cases[i], &result);
    int floor_status = s21_floor(invalid_cases[i], &result);
    int negate_status = s21_negate(invalid_cases[i], &result);

    ck_assert(round_status == 0 || round_status == 1);
    ck_assert(truncate_status == 0 || truncate_status == 1);
    ck_assert(floor_status == 0 || floor_status == 1 || floor_status == 2);
    ck_assert(negate_status == 0 || negate_status == 1);
  }
}
END_TEST

START_TEST(test_memory_safety) {
  s21_decimal *null_dec = NULL;
  s21_decimal valid = {{123, 0, 0, 0}};

  ck_assert_int_eq(1, s21_round(valid, null_dec));
  ck_assert_int_eq(1, s21_truncate(valid, null_dec));
  ck_assert_int_eq(1, s21_floor(valid, null_dec));
  ck_assert_int_eq(1, s21_negate(valid, null_dec));
}
END_TEST

Suite *suite_other(void) {
  Suite *s = suite_create("s21_other");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_round_positive);
  tcase_add_test(tc, test_round_negative);
  tcase_add_test(tc, test_round_bankers_even);
  tcase_add_test(tc, test_round_bankers_odd);
  tcase_add_test(tc, test_round_max_scale);
  tcase_add_test(tc, test_round_overflow);
  tcase_add_test(tc, test_round_bankers_all_cases);
  tcase_add_test(tc, test_round_large_numbers);
  tcase_add_test(tc, test_round_precision_loss);

  tcase_add_test(tc, test_truncate_positive);
  tcase_add_test(tc, test_truncate_negative);
  tcase_add_test(tc, test_truncate_zero_scale);
  tcase_add_test(tc, test_truncate_small_fraction);
  tcase_add_test(tc, test_truncate_extreme_values);
  tcase_add_test(tc, test_truncate_negative_zero);

  tcase_add_test(tc, test_floor_positive);
  tcase_add_test(tc, test_floor_negative);
  tcase_add_test(tc, test_floor_min_negative);
  tcase_add_test(tc, test_floor_negative_precision);
  tcase_add_test(tc, test_floor_overflow_protection);

  tcase_add_test(tc, test_negate_positive);
  tcase_add_test(tc, test_negate_negative);
  tcase_add_test(tc, test_negate_zero);
  tcase_add_test(tc, test_negate_max_value);
  tcase_add_test(tc, test_negate_preserves_scale);
  tcase_add_test(tc, test_negate_double_negate);

  tcase_add_test(tc, test_stress_random);
  tcase_add_test(tc, test_invalid_decimal_state);
  tcase_add_test(tc, test_memory_safety);

  suite_add_tcase(s, tc);
  return s;
}