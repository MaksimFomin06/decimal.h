#include <check.h>

#include "s21_decimal.h"

START_TEST(test_s21_add1) {
  s21_decimal value1 = {{0x1, 0x0, 0x0, 0x0}};
  s21_decimal value2 = {{0x1, 0x0, 0x0, 0x0}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}

START_TEST(test_s21_add2) {
  s21_decimal value1 = {{0x1, 0x0, 0x1, 0x0}};
  s21_decimal value2 = {{0x1, 0x0, 0x1, 0x0}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 2);
  ck_assert_int_eq(res.bits[3], 0);
}

START_TEST(test_s21_add3) {
  s21_decimal value1 = {{0x1, 0x0, 0x0, 0x80000000}};
  s21_decimal value2 = {{0x1, 0x0, 0x0, 0x0}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}

START_TEST(test_s21_add4) {
  s21_decimal value1 = {{0x1, 0x0, 0x0, 0x0}};
  s21_decimal value2 = {{0x1, 0x0, 0x0, 0x80000000}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}

START_TEST(test_s21_add5) {
  s21_decimal value1 = {{123, 0, 0, 0}};
  s21_decimal value2 = {{456, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 579);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_s21_add6) {
  s21_decimal value1 = {{123, 0, 0, 2 << 16}};
  s21_decimal value2 = {{456, 0, 0, 2 << 16}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 579);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq((res.bits[3] >> 16) & 0xFF, 2);
}
END_TEST

START_TEST(test_s21_add7) {
  s21_decimal value1 = {{100, 0, 0, 0x80000000}};
  s21_decimal value2 = {{50, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 50);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], -2147483648);
}
END_TEST

START_TEST(test_s21_add8) {
  s21_decimal value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value2 = {{1, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(1, s21_add(value1, value2, &res));
}
END_TEST

START_TEST(test_s21_add9) {
  s21_decimal value1 = {{0, 0, 0, 0x80000000}};
  s21_decimal value2 = {{0, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_add(value1, value2, &res));

  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

START_TEST(test_s21_add10) {
  s21_decimal value1 = {{123456, 0, 0, 4 << 16}};
  s21_decimal value2 = {{456, 0, 0, 2 << 16}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
}
END_TEST

START_TEST(test_s21_add11) {
  s21_decimal value1 = {{123456, 0, 0, 1 << 16}};
  s21_decimal value2 = {{456783, 0, 0, 5 << 16}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
}
END_TEST

START_TEST(test_s21_add12) {
  s21_decimal value1 = {{123456, 0, 0, 12 << 16}};
  s21_decimal value2 = {{456783, 0, 0, 5 << 16}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
}
END_TEST

START_TEST(test_s21_add13) {
  s21_decimal value1 = {{123456, 0, 0, 2 << 16}};
  s21_decimal value2 = {{456783, 0, 0, 15 << 16}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
}
END_TEST

START_TEST(test_s21_add14) {
  s21_decimal value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal value2 = {{1, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_add(value1, value2, &res));
}
END_TEST

START_TEST(test_s21_add15) {
  s21_decimal value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFF, 0x80000000}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(2, s21_add(value1, value2, &res));  // 2 when 1
}
END_TEST
// 10 / 2 = 5
START_TEST(test_s21_div1) {
  s21_decimal value1 = {{10, 0, 0, 0}};
  s21_decimal value2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_div(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 5);
}
END_TEST

// -20 / 4 = -5
START_TEST(test_s21_div2) {
  s21_decimal value1 = {{20, 0, 0, 0x80000000}};
  s21_decimal value2 = {{4, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_div(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 5);
  ck_assert_int_eq(res.bits[3], -2147483648);
}
END_TEST

// 5 / 2 = 2.5
START_TEST(test_s21_div3) {
  s21_decimal value1 = {{5, 0, 0, 0}};
  s21_decimal value2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_div(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 25);                // 2.5 = 25 with scale 1
  ck_assert_int_eq((res.bits[3] >> 16) & 0xFF, 1);  // scale = 1
}
END_TEST

START_TEST(test_s21_div4) {
  s21_decimal value1 = {{123, 0, 0, 0}};
  s21_decimal value2 = {{0, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(3, s21_div(value1, value2, &res));  // 3
}
END_TEST

START_TEST(test_s21_div5) {
  s21_decimal value1 = {{1000000000, 0, 0, 0}};  // 1e9
  s21_decimal value2 = {{10, 0, 0, 0}};          // 10
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_div(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 100000000);  // 1e8
}
END_TEST

START_TEST(test_s21_div_small_dividend_repeat) {
  s21_decimal a = {{1u, 0u, 0u, 0u}}; /* 1 */
  s21_decimal b = {{3u, 0u, 0u, 0u}}; /* 3 */
  s21_decimal res = {{0u, 0u, 0u, 0u}};

  ck_assert_int_eq(S21_OK, s21_div(a, b, &res));

  int out_scale = (res.bits[3] >> 16) & 0xFF;
  ck_assert(out_scale > 0);
  ck_assert(res.bits[0] != 0u || res.bits[1] != 0u || res.bits[2] != 0u);
}

START_TEST(test_s21_div_large_by_small) {
  s21_decimal a = {{0xFFFFFFFFu, 0xFFFFFFFFu, 0x00FFFFFFu, 0u}};
  s21_decimal b = {{12345u, 0u, 0u, 0u}};
  s21_decimal res = {{0u, 0u, 0u, 0u}};

  int rc = s21_div(a, b, &res);
  ck_assert_int_eq(S21_OK, rc);

  ck_assert(res.bits[0] != 0u || res.bits[1] != 0u || res.bits[2] != 0u);
}
END_TEST
// 2 * 3 = 6
START_TEST(test_s21_mul1) {
  s21_decimal value1 = {{2, 0, 0, 0}};
  s21_decimal value2 = {{3, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_mul(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 6);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

// -5 * 4 = -20
START_TEST(test_s21_mul2) {
  s21_decimal value1 = {{5, 0, 0, 0x80000000}};
  s21_decimal value2 = {{4, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_mul(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 20);
  ck_assert_int_eq(res.bits[3], -2147483648);
}
END_TEST

// 1.2 * 3.4 = 4.08
START_TEST(test_s21_mul3) {
  s21_decimal value1 = {{12, 0, 0, 1 << 16}};  // 1.2
  s21_decimal value2 = {{34, 0, 0, 1 << 16}};  // 3.4
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_mul(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 408);               // 4.08
  ck_assert_int_eq((res.bits[3] >> 16) & 0xFF, 2);  // scale = 2
}
END_TEST

START_TEST(test_s21_mul4) {
  s21_decimal value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(1, s21_mul(value1, value2, &res));  // 1 = overflow
}
END_TEST

START_TEST(test_s21_mul5) {
  s21_decimal value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(1, s21_mul(value1, value2, &res));  // 1 = overflow
}
END_TEST

START_TEST(test_s21_mul6) {
  s21_decimal value1 = {{323134212, 23, 0, 12 << 16}};  // 1.2
  s21_decimal value2 = {{3321324, 32, 0, 15 << 16}};    // 3.4
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_mul(value1, value2, &res));
}
END_TEST

START_TEST(test_s21_mul_enter_while_body) {
  s21_decimal a = {{0, 0, 0xFFFFFFFF, 0}};

  s21_decimal b = {{0, 2, 0, 1 << 16}};
  s21_decimal res = {{0, 0, 0, 0}};

  int rc = s21_mul(a, b, &res);
  ck_assert_int_eq(1, rc);
}
END_TEST

START_TEST(test_s21_mul_overflow_pos) {
  s21_decimal a = {{0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u}};
  s21_decimal b = {{2u, 0u, 0u, 0u}};
  s21_decimal res = {{0, 0, 0, 0}};
  int rc = s21_mul(a, b, &res);
  ck_assert_int_eq(1, rc);
}
END_TEST

START_TEST(test_s21_mul_overflow_neg) {
  s21_decimal a = {{0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0x80000000u}};
  s21_decimal b = {{2u, 0u, 0u, 0u}};
  s21_decimal res = {{0, 0, 0, 0}};

  int rc = s21_mul(a, b, &res);
  ck_assert_int_eq(2, rc);
}
END_TEST
START_TEST(test_s21_sub1) {
  s21_decimal value1 = {{0x1, 0x0, 0x0, 0x0}};
  s21_decimal value2 = {{0x1, 0x0, 0x0, 0x0}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_sub(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
START_TEST(test_s21_sub2) {
  s21_decimal value1 = {{123, 0, 0, 2 << 16}};
  s21_decimal value2 = {{23, 0, 0, 2 << 16}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_sub(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 1);
  ck_assert_int_eq((res.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(test_s21_sub3) {
  s21_decimal value1 = {{10, 0, 0, 0x80000000}};
  s21_decimal value2 = {{5, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_sub(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 15);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], -2147483648);
}
END_TEST

START_TEST(test_s21_sub4) {
  s21_decimal value1 = {{0, 0, 0, 0}};
  s21_decimal value2 = {{0, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_sub(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

START_TEST(test_s21_sub5) {
  s21_decimal value1 = {{0, 0, 0, 0}};
  s21_decimal value2 = {{0, 0, 0, 0x80000000}};
  s21_decimal res = {{0, 0, 0, 0}};

  ck_assert_int_eq(0, s21_sub(value1, value2, &res));
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

START_TEST(test_s21_check_null_bits) {
  s21_decimal zero_value = {};
  s21_decimal non_zero_value = {.bits = {1}};

  // Проверяем случай нуля
  ck_assert_msg(s21_check_null_bits(zero_value) == 1,
                "Ошибка: ноль не распознан функцией.");

  // Проверяем случай ненулевых битов
  ck_assert_msg(s21_check_null_bits(non_zero_value) == 0,
                "Ошибка: функция неверно распознала ненулевые биты.");
}
END_TEST

// Тест для функции s21_u96_div_pow10
START_TEST(test_s21_u96_div_pow10) {
  u96 input = {.bits = {123456789, 0, 0}};  // Исходное значение
  u96 expected_result = {
      .bits = {12345678, 0, 0}};  // Ожидаемый результат деления на 10^1
  u96 actual_result = s21_u96_div_pow10(input, 1);  // Выполняем деление на 10^1

  // Проверяем совпадение результата
  ck_assert_msg(actual_result.bits[0] == expected_result.bits[0],
                "Ошибка: неправильное деление на степень 10.");

  // Дополнительные тесты с разными степенями
  input.bits[0] = 1234567890;
  expected_result.bits[0] = 123456789;
  actual_result = s21_u96_div_pow10(input, 1);
  ck_assert_msg(actual_result.bits[0] == expected_result.bits[0],
                "Ошибка: другое число неправильно делится на 10.");

  input.bits[0] = 1234567890;
  expected_result.bits[0] = 12345678;
  actual_result = s21_u96_div_pow10(input, 2);
  ck_assert_msg(actual_result.bits[0] == expected_result.bits[0],
                "Ошибка: неправильный результат деления на 10^2.");
}
END_TEST

START_TEST(test_s21_add_overflow_negative) {
  s21_decimal max_negative = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal one_negative = {{1, 0, 0, 0x80000000}};
  s21_decimal res;

  // -MAX - 1 = должно быть отрицательное переполнение (2)
  ck_assert_int_eq(2, s21_add(max_negative, one_negative, &res));
}
END_TEST

START_TEST(test_s21_sub_overflow_positive) {
  s21_decimal max_positive = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal one_negative = {{1, 0, 0, 0x80000000}};
  s21_decimal res;

  // MAX - (-1) = MAX + 1 = положительное переполнение (1)
  ck_assert_int_eq(1, s21_sub(max_positive, one_negative, &res));
}
END_TEST

START_TEST(test_s21_div_overflow) {
  s21_decimal huge_number = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal tiny_number = {{1, 0, 0, 28 << 16}};  // 1e-28
  s21_decimal res;

  // Очень большое число / очень маленькое = переполнение
  ck_assert_int_eq(1, s21_div(huge_number, tiny_number, &res));
}
END_TEST

START_TEST(test_s21_mul_overflow_negative_correct) {
  s21_decimal max_negative = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal two = {{2, 0, 0, 0}};
  s21_decimal res;

  // -MAX * 2 = отрицательное переполнение (должно возвращать 2)
  ck_assert_int_eq(2, s21_mul(max_negative, two, &res));
}
END_TEST

// Тесты для переполнения в сложении
START_TEST(test_s21_add_overflow_positive) {
  s21_decimal max_positive = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal res;

  // MAX + 1 = положительное переполнение (1)
  ck_assert_int_eq(1, s21_add(max_positive, one, &res));
}
END_TEST

START_TEST(test_s21_sub_overflow_negative) {
  s21_decimal max_negative = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal res;

  // -MAX - 1 = -MAX-1 = отрицательное переполнение (2)
  ck_assert_int_eq(2, s21_sub(max_negative, one, &res));
}
END_TEST

// Тесты для переполнения в умножении
START_TEST(test_s21_mul_overflow_positive) {
  s21_decimal max_positive = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal two = {{2, 0, 0, 0}};
  s21_decimal res;

  // MAX * 2 = положительное переполнение (1)
  ck_assert_int_eq(1, s21_mul(max_positive, two, &res));
}
END_TEST

START_TEST(test_s21_mul_overflow_negative) {
  s21_decimal max_negative = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal two = {{2, 0, 0, 0}};
  s21_decimal res;

  // -MAX * 2 = отрицательное переполнение (2)
  ck_assert_int_eq(2, s21_mul(max_negative, two, &res));
}
END_TEST

// Тесты для переполнения в делении
START_TEST(test_s21_div_overflow_positive) {
  s21_decimal huge_number = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal tiny_number = {{1, 0, 0, 28 << 16}};  // 1e-28
  s21_decimal res;

  // Очень большое число / очень маленькое = положительное переполнение (1)
  ck_assert_int_eq(1, s21_div(huge_number, tiny_number, &res));
}
END_TEST

START_TEST(test_s21_div_overflow_negative) {
  s21_decimal huge_negative = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal tiny_number = {{1, 0, 0, 28 << 16}};  // 1e-28
  s21_decimal res;

  // Очень большое отрицательное число / очень маленькое = отрицательное
  // переполнение (2)
  ck_assert_int_eq(2, s21_div(huge_negative, tiny_number, &res));
}
END_TEST

START_TEST(test_s21_div_by_zero) {
  s21_decimal value = {{123, 0, 0, 0}};
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_decimal res;

  // Деление на ноль (3)
  ck_assert_int_eq(3, s21_div(value, zero, &res));
}
END_TEST

// Тест для масштабного переполнения в умножении
START_TEST(test_s21_mul_scale_overflow) {
  s21_decimal value1 = {{123, 0, 0, 15 << 16}};  // scale 15
  s21_decimal value2 = {{456, 0, 0, 14 << 16}};  // scale 14
  s21_decimal res;

  // 15 + 14 = 29 > 28 = переполнение
  ck_assert_int_eq(
      1, s21_mul(value1, value2, &res));  // Должен вернуть 1 для положительного
}
END_TEST

Suite *suite_s21_sub(void) {
  Suite *suite = suite_create("suite_s21_sub");
  TCase *tcase_core = tcase_create("tc_s21_sub");

  tcase_add_test(tcase_core, test_s21_sub1);
  tcase_add_test(tcase_core, test_s21_sub2);
  tcase_add_test(tcase_core, test_s21_sub3);
  tcase_add_test(tcase_core, test_s21_sub4);
  tcase_add_test(tcase_core, test_s21_sub5);
  tcase_add_test(tcase_core, test_s21_sub_overflow_positive);
  tcase_add_test(tcase_core, test_s21_sub_overflow_positive);
  tcase_add_test(tcase_core, test_s21_sub_overflow_negative);

  suite_add_tcase(suite, tcase_core);
  return suite;
}
Suite *suite_s21_mul(void) {
  Suite *suite = suite_create("suite_s21_mul");
  TCase *tcase_core = tcase_create("tc_s21_mul");

  tcase_add_test(tcase_core, test_s21_mul1);
  tcase_add_test(tcase_core, test_s21_mul2);
  tcase_add_test(tcase_core, test_s21_mul3);
  tcase_add_test(tcase_core, test_s21_mul4);
  tcase_add_test(tcase_core, test_s21_mul5);
  tcase_add_test(tcase_core, test_s21_mul6);
  tcase_add_test(tcase_core, test_s21_mul_enter_while_body);
  tcase_add_test(tcase_core, test_s21_mul_overflow_pos);
  tcase_add_test(tcase_core, test_s21_mul_overflow_neg);
  tcase_add_test(tcase_core, test_s21_mul_overflow_negative_correct);
  tcase_add_test(tcase_core, test_s21_mul_overflow_positive);
  tcase_add_test(tcase_core, test_s21_mul_overflow_negative);
  tcase_add_test(tcase_core, test_s21_mul_scale_overflow);

  suite_add_tcase(suite, tcase_core);
  return suite;
}
Suite *suite_s21_div(void) {
  Suite *suite = suite_create("suite_s21_div");
  TCase *tcase_core = tcase_create("tc_s21_div");

  tcase_add_test(tcase_core, test_s21_div1);
  tcase_add_test(tcase_core, test_s21_div2);
  tcase_add_test(tcase_core, test_s21_div3);
  tcase_add_test(tcase_core, test_s21_div4);
  tcase_add_test(tcase_core, test_s21_div5);
  tcase_add_test(tcase_core, test_s21_div_small_dividend_repeat);
  tcase_add_test(tcase_core, test_s21_div_large_by_small);
  tcase_add_test(tcase_core, test_s21_div_overflow);
  tcase_add_test(tcase_core, test_s21_div_overflow_positive);
  tcase_add_test(tcase_core, test_s21_div_overflow_negative);
  tcase_add_test(tcase_core, test_s21_div_by_zero);

  suite_add_tcase(suite, tcase_core);
  return suite;
}

Suite *Arithmetic_suite(void) {
  Suite *suite = suite_create("arithmetic");
  TCase *tcase_core = tcase_create("Core");

  tcase_add_test(tcase_core, test_s21_add1);
  tcase_add_test(tcase_core, test_s21_add2);
  tcase_add_test(tcase_core, test_s21_add3);
  tcase_add_test(tcase_core, test_s21_add4);
  tcase_add_test(tcase_core, test_s21_add5);
  tcase_add_test(tcase_core, test_s21_add6);
  tcase_add_test(tcase_core, test_s21_add7);
  tcase_add_test(tcase_core, test_s21_add8);
  tcase_add_test(tcase_core, test_s21_add9);
  tcase_add_test(tcase_core, test_s21_add10);
  tcase_add_test(tcase_core, test_s21_add11);
  tcase_add_test(tcase_core, test_s21_add12);
  tcase_add_test(tcase_core, test_s21_add13);
  tcase_add_test(tcase_core, test_s21_add14);
  tcase_add_test(tcase_core, test_s21_add15);
  tcase_add_test(tcase_core, test_s21_add_overflow_negative);
  tcase_add_test(tcase_core, test_s21_add_overflow_positive);
  tcase_add_test(tcase_core, test_s21_add_overflow_negative);

  suite_add_tcase(suite, tcase_core);
  return suite;
}

Suite *create_tests() {
  Suite *suite = suite_create("Tests for Support Functions");
  TCase *core_case = tcase_create("Core");

  tcase_add_test(core_case, test_s21_check_null_bits);
  tcase_add_test(core_case, test_s21_u96_div_pow10);

  suite_add_tcase(suite, core_case);
  return suite;
}