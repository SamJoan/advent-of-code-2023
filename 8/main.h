#include <stdint.h>
#include <stdlib.h>

typedef struct Node {
    struct Node *left;
    char *left_label;
    struct Node *right;
    char *right_label;
    char *label;
} Node;

typedef struct {
    Node *starting_node;
    char *instructions;
    Node **nodes;
    size_t len;
} Network;

Network *network_parse(const char *filename);
uint64_t solve_part_1(char *filename);
void network_free(Network *n);
