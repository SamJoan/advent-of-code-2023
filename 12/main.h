#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char *springs;
    int *dsg;
} Record;

typedef struct {
    Record **data;
    size_t len;
} ConditionRecords;

ConditionRecords *parse_records(const char *filename, bool unfold);
uint64_t solve_part_1(char *filename);
void cr_free(ConditionRecords *cr);
size_t dsglen(int *dsg);
uint64_t solve_part_2(char *filename);
