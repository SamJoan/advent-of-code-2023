#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_get_calibration_value) {
    mu_check(get_calibration_value("1abc2") == 12);
    mu_check(get_calibration_value("pqr3stu8vwx") == 38);
    mu_check(get_calibration_value("a1b2c3d4e5f") == 15);
    mu_check(get_calibration_value("treb7uchet") == 77);
}

MU_TEST(test_sum_allocation_values) {
    char *lines = "1abc2\npqr3stu8vwx\na1b2c3d4e5f\ntreb7uchet";
    mu_check(sum_calibration_values(lines) == 142);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_get_calibration_value);
    MU_RUN_TEST(test_sum_allocation_values);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
