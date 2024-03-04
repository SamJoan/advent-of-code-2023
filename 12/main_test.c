#include "lib/util.h"
#include "main.h"
#include "minunit.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

MU_TEST(test_parse_records) {
    ConditionRecords *records = parse_records("input_test.txt", false);
    mu_check(records->len == 6);
    mu_check(strcmp(records->data[0]->springs, "???.###") == 0);
    mu_check(dsglen(records->data[0]->dsg) == 3);
    mu_check(records->data[0]->dsg[0] == 1);
    mu_check(records->data[0]->dsg[1] == 1);
    mu_check(records->data[0]->dsg[2] == 3);
    mu_check(records->data[0]->dsg[3] == -1);

    mu_check(strcmp(records->data[5]->springs, "?###????????") == 0);
    mu_check(dsglen(records->data[5]->dsg) == 3);
    mu_check(records->data[5]->dsg[0] == 3);
    mu_check(records->data[5]->dsg[1] == 2);
    mu_check(records->data[5]->dsg[2] == 1);
    mu_check(records->data[5]->dsg[3] == -1);

    cr_free(records);
}

MU_TEST(test_parse_records_unfold) {
    ConditionRecords *records = parse_records("input_test.txt", true);
    mu_check(records->len == 6);
    mu_check(strcmp(records->data[0]->springs, "???.###????.###????.###????.###????.###") == 0);
    mu_check(dsglen(records->data[0]->dsg) == 15);
    mu_check(records->data[0]->dsg[0] == 1);
    mu_check(records->data[0]->dsg[1] == 1);
    mu_check(records->data[0]->dsg[2] == 3);
    mu_check(records->data[0]->dsg[3] == 1);
    mu_check(records->data[0]->dsg[4] == 1);
    mu_check(records->data[0]->dsg[5] == 3);
    mu_check(records->data[0]->dsg[12] == 1);
    mu_check(records->data[0]->dsg[13] == 1);
    mu_check(records->data[0]->dsg[14] == 3);
    mu_check(records->data[0]->dsg[15] == -1);

    mu_check(strcmp(records->data[5]->springs, "?###??????????###??????????###??????????###??????????###????????") == 0);
    mu_check(dsglen(records->data[5]->dsg) == 15);
    mu_check(records->data[5]->dsg[0] == 3);
    mu_check(records->data[5]->dsg[1] == 2);
    mu_check(records->data[5]->dsg[2] == 1);
    mu_check(records->data[5]->dsg[3] == 3);
    mu_check(records->data[5]->dsg[15] == -1);

    cr_free(records);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt");

    mu_check(result == 21);
}

MU_TEST(test_solve_part_2) {
    uint64_t result = solve_part_2("input_test.txt");

    mu_check(result == 525152);
}

MU_TEST(test_hashmap) {
    char *springs = "????.#...#...";
    int dsg[] = { 4, 1, 1, -1 };

    FunctionCall *f1 = function_call_init(springs, dsg, 0);
    FunctionCall *f2 = function_call_init(springs, dsg, 1);

    HashMap *h = hashmap_init(1000000, function_call_hash, function_call_cmp);
    hashmap_add(h, f1);

    mu_check(hashmap_in(h, f1) == true);
    mu_check(hashmap_in(h, f2) == false);

    // Ooh biscuits forgot to add the value! XD
}

MU_TEST_SUITE(test_suite) {
    /*MU_RUN_TEST(test_parse_records);*/
    /*MU_RUN_TEST(test_parse_records_unfold);*/
    /*MU_RUN_TEST(test_solve_part_1);*/
    /*MU_RUN_TEST(test_solve_part_2);*/
    MU_RUN_TEST(test_hashmap);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
