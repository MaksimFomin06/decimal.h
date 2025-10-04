#include "s21_decimal.h"

int s21_add(s21_decimal arg1, s21_decimal arg2, s21_decimal *result) {
  int status = S21_OK;
  if (!result) return status;

  s21_decimal_init(result);
  int scale1 = s21_get_scale_2(arg1);
  bool sameSign = s21_get_sign(arg1) == s21_get_sign(arg2);
  s21_bank_rounding(&arg1, &arg2);
  scale1 = s21_get_scale_2(arg1);

  uint32_t mntssA[3] = {arg1.bits[0], arg1.bits[1], arg1.bits[2]};
  uint32_t mntssB[3] = {arg2.bits[0], arg2.bits[1], arg2.bits[2]};
  uint32_t mntssR[3] = {0, 0, 0};
  int finalScale = scale1;
  int finalSign = 0;

  if (sameSign) {
    status = handle_addition(mntssA, mntssB, mntssR, &finalScale, &finalSign,
                             s21_get_sign(arg1));
  } else {
    handle_subtraction(mntssA, mntssB, mntssR, &finalSign, s21_get_sign(arg1),
                       s21_get_sign(arg2));
  }

  if (status == S21_OK) {
    s21_trim_tz(mntssR, &finalScale);
    result->bits[0] = mntssR[0];
    result->bits[1] = mntssR[1];
    result->bits[2] = mntssR[2];
    s21_set_scale_2(result, finalScale);
    s21_set_sign_2(result, finalSign);
  }

  return status;
}

int s21_div(s21_decimal dividend, s21_decimal divisor, s21_decimal *output) {
  int status = S21_OK;
  if (!output) return status;

  s21_decimal_init(output);
  uint32_t divisorArr[3] = {divisor.bits[0], divisor.bits[1], divisor.bits[2]};

  if (s21_is_96_zero(divisorArr)) {
    return S21_DIV_ZERO;
  }

  uint32_t dividendArr[3] = {dividend.bits[0], dividend.bits[1],
                             dividend.bits[2]};
  if (s21_is_96_zero(dividendArr)) {
    return S21_OK;
  }

  int resultSign = s21_get_sign(dividend) ^ s21_get_sign(divisor);
  s21_bank_rounding(&dividend, &divisor);

  int resultScale = s21_get_scale_2(dividend) - s21_get_scale_2(divisor);
  if (resultScale < 0) resultScale = 0;

  dividendArr[0] = dividend.bits[0];
  dividendArr[1] = dividend.bits[1];
  dividendArr[2] = dividend.bits[2];
  divisorArr[0] = divisor.bits[0];
  divisorArr[1] = divisor.bits[1];
  divisorArr[2] = divisor.bits[2];

  uint32_t quotient[3] = {0};
  status = handle_division(dividendArr, divisorArr, quotient, &resultScale);

  if (status == S21_OK) {
    status = check_overflow(quotient, resultScale, resultSign);
  }

  if (status == S21_OK) {
    s21_trim_tz(quotient, &resultScale);
    output->bits[0] = quotient[0];
    output->bits[1] = quotient[1];
    output->bits[2] = quotient[2];
    s21_set_scale_2(output, resultScale);
    s21_set_sign_2(output, resultSign);
  }

  return status;
}

int s21_mul(s21_decimal factor1, s21_decimal factor2, s21_decimal *product) {
  if (product == NULL) return -1;

  int status = S21_OK;
  int scaleFactor1 = s21_get_scale_2(factor1);
  int scaleFactor2 = s21_get_scale_2(factor2);

  if (scaleFactor1 + scaleFactor2 > 28) {
    int sign1 = s21_get_sign(factor1);
    int sign2 = s21_get_sign(factor2);
    int finalSign = sign1 ^ sign2;
    return finalSign ? S21_INF_NEG : S21_INF_POS;
  }

  s21_decimal_init(product);
  int scale1 = s21_get_scale_2(factor1);
  int scale2 = s21_get_scale_2(factor2);
  int sign1 = s21_get_sign(factor1);
  int sign2 = s21_get_sign(factor2);
  int finalSign = sign1 ^ sign2;
  int totalScale = scale1 + scale2;

  uint64_t intermediate[6] = {0, 0, 0, 0, 0, 0};

  multiply_factors(factor1, factor2, intermediate);
  normalize_intermediate(intermediate);

  if (scale_down_result(intermediate, &totalScale)) {
    return finalSign ? S21_INF_NEG : S21_INF_POS;
  }

  uint32_t reducedResult[3] = {(uint32_t)intermediate[0],
                               (uint32_t)intermediate[1],
                               (uint32_t)intermediate[2]};

  status = check_result_overflow(reducedResult, finalSign);

  if (status == S21_OK) {
    s21_trim_tz(reducedResult, &totalScale);
    product->bits[0] = reducedResult[0];
    product->bits[1] = reducedResult[1];
    product->bits[2] = reducedResult[2];
    s21_set_scale_2(product, totalScale);
    s21_set_sign_2(product, finalSign);
  }

  return status;
}

int s21_sub(s21_decimal minuend, s21_decimal subtrahend,
            s21_decimal *difference) {
  if (!difference) return S21_OK;
  s21_decimal invertedSubtrahend = subtrahend;
  s21_set_sign_2(&invertedSubtrahend, !s21_get_sign(subtrahend));
  return s21_add(minuend, invertedSubtrahend, difference);
}