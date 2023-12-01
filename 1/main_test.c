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

MU_TEST(test_get_calibration_value_silly) {
    /*mu_check(get_calibration_value("two1nine") == 29);*/
    /*mu_check(get_calibration_value("eightwothree") == 83);*/
    /*mu_check(get_calibration_value("abcone2threexyz") == 13);*/
    /*mu_check(get_calibration_value("xtwone3four") == 24);*/
    /*mu_check(get_calibration_value("4nineeightseven2") == 42);*/
    /*mu_check(get_calibration_value("zoneight234") == 14);*/
    /*mu_check(get_calibration_value("7pqrstsixteen") == 76);*/
    /*mu_check(get_calibration_value("hfbnlvhd76kmxf414pm") == 74);*/

    mu_check(get_calibration_value("7pqrstsixteen9six") == 76);
}

MU_TEST(test_sum_allocation_values) {
    char *lines = "1abc2\npqr3stu8vwx\na1b2c3d4e5f\ntreb7uchet";
    mu_check(sum_calibration_values(lines) == 142);
}

MU_TEST(test_sum_allocation_values_silly) {
    char *lines = "two1nine\neightwothree\nabcone2threexyz\nxtwone3four\n4nineeightseven2\nzoneight234\n7pqrstsixteen";
    mu_check(sum_calibration_values(lines) == 281);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_get_calibration_value);
    MU_RUN_TEST(test_get_calibration_value_silly);
    MU_RUN_TEST(test_sum_allocation_values);
    MU_RUN_TEST(test_sum_allocation_values_silly);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
