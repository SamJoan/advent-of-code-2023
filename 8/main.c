#define _GNU_SOURCE
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Network *network_init() {
    Network *n = malloc(sizeof(Network));
    n->instructions = NULL;
    n->starting_node = NULL;

    return n;
}

void parse_lr(char *str_in, char **left, char**right, int label_len) {
    char *str = strdup(str_in);
    char *str_orig = str;
    char *token;
    int i = 0;
    while((token = strtok_r(str, ",", &str))) {
        if(i == 0) {
            *left = strndup(token + 2, label_len);
        } else if(i == 1) {
            *right = strndup(token + 1, label_len);
        }
        
        i++;
    }

    free(str_orig);
}

Node *node_init(char *label, char *left, char *right) {
    Node *node = malloc(sizeof(Node));
    node->label = label;
    node->left_label = left;
    node->right_label = right;

    return node;
}

Node *node_parse(char *line) {
    char *line_dup = strdup(line);
    char *line_dup_orig = line_dup;
    char *token = NULL;

    size_t label_len = 3;

    int i = 0;
    char *left = NULL;
    char *right = NULL;
    char *label = NULL;
    while((token = strtok_r(line, "=", &line))) {
        if(i == 0) {
            label = strndup(token, label_len);
        } else if (i == 1) {
            parse_lr(token, &left, &right, label_len);
        } else {
            printf("Error splitting string.\n");
            exit(1);
        }
        i++;
    }

    Node * node = node_init(label, left, right);
    free(line_dup_orig);

    return node;
}

void network_free(Network *n) {
    for(int i = 0; i < n->len; i++) {
        Node *node = n->nodes[i];
        free(node->label);
        free(node->left_label);
        free(node->right_label);

        free(n->nodes[i]);
    }

    free(n->nodes);
    free(n->instructions);
    free(n);
}

Network *network_parse(const char *filename) {
    Network *n = network_init();

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int result = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    Node **nodes = NULL;
    size_t nb_nodes = 0;

    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        if(i == 0) {
            n->instructions = strndup(line, strlen(line) - 1);
        } else if(i > 1) {
            Node *new_node = node_parse(line);
            nb_nodes++;
            nodes = realloc(nodes, sizeof(Node*) * nb_nodes);
            nodes[nb_nodes - 1] = new_node;

            if(strcmp(new_node->label, "AAA") == 0) {
                n->starting_node = new_node;
            }

        }

        i++;
    }

    for(int i = 0; i < nb_nodes; i++) {
        for(int j = 0; j < nb_nodes; j++) {
            Node *node = nodes[i];
            Node *new_node = nodes[j];

            if(strcmp(node->left_label, new_node->label) == 0) {
                node->left = new_node;
            }
            if(strcmp(node->right_label, new_node->label) == 0) {
                node->right = new_node;
            }
        }
    }

    fclose(fp);
    if (line)
        free(line);

    n->nodes = nodes;
    n->len = nb_nodes;

    return n;
}

uint64_t solve_part_1(char *filename) {
    Network *n = network_parse(filename);
    Node *cur_node = n->starting_node;
    int nb = 0;
    while(strcmp(cur_node->label, "ZZZ") != 0) {
        char inst = n->instructions[nb % strlen(n->instructions)];
        cur_node = inst == 'L' ? cur_node->left : cur_node->right;
        nb++;
    }

    network_free(n);

    return nb;
}

int main(int argc, char *argv[]) {

    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {

        uint64_t result = solve_part_1("input.txt");

        printf("%lu\n", result);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
