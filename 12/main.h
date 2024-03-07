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

typedef struct {
    char *springs;
    int *dsg;
    int group_size;
} FunctionCall;

ConditionRecords *parse_records(const char *filename, bool unfold);
size_t dsglen(int *dsg);
uint64_t function_call_hash(void *function_call_in);
bool function_call_cmp(void *func_a, void *func_b);
FunctionCall *function_call_init(char *springs, int *dsg, int group_size);
void function_call_free(FunctionCall *f);
uint64_t solve_part_1(char *filename);
uint64_t solve_part_2(char *filename);
void cr_free(ConditionRecords *cr);
