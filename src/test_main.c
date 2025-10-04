#include <check.h>

#include "s21_decimal.h"

Suite *Arithmetic_suite(void);
Suite *suite_s21_div(void);
Suite *suite_s21_mul(void);
Suite *suite_s21_sub(void);
Suite *create_tests(void);
Suite *conversion_suite(void);
Suite *comparison_suite(void);
Suite *suite_other(void);
Suite *help_functions(void);

void run_testcase(Suite *testcase, int *total_failed) {
  static int counter = 1;
  printf("Test suite %d: ", counter++);

  SRunner *sr = srunner_create(testcase);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  int failed = srunner_ntests_failed(sr);
  *total_failed += failed;

  if (failed == 0) {
    printf("✅ PASSED\n");
  } else {
    printf("❌ FAILED (%d tests failed)\n", failed);
  }

  srunner_free(sr);
  printf("\n");
}

int main(void) {
  Suite *test_suites[] = {Arithmetic_suite(), create_tests(),
                          suite_s21_mul(),    suite_s21_div(),
                          suite_s21_sub(),    suite_other(),
                          conversion_suite(), comparison_suite(),
                          help_functions(),   NULL};

  printf("=== S21_DECIMAL TESTS ===\n\n");

  int total_failed = 0;

  for (int i = 0; test_suites[i] != NULL; i++) {
    run_testcase(test_suites[i], &total_failed);
  }

  printf("=== ALL TESTS COMPLETED ===\n");

  if (total_failed == 0) {
    printf("🎉 ALL TESTS PASSED! ✅\n");
  } else {
    printf("💥 TESTS FAILED: %d test(s) failed ❌\n", total_failed);
  }

  return total_failed == 0 ? 0 : 1;
}