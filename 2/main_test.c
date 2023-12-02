#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_game) {
    /*mu_check(get_calibration_value("1abc2") == 12);*/
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_parse_game);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
