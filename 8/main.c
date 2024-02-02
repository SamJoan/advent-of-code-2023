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

Node *starting_node_pt1(Network *n) {
    for(int i = 0; i < n->len; i++) {
        Node *node = n->nodes[i];
        if(strcmp(node->label, "AAA") == 0) {
            return node;
        }
    }

    printf("No starting node\n");
    exit(1);
}

uint64_t solve_part_1(char *filename) {
    Network *n = network_parse(filename);
    Node *cur_node = starting_node_pt1(n);
    int nb = 0;
    while(strcmp(cur_node->label, "ZZZ") != 0) {
        char inst = n->instructions[nb % strlen(n->instructions)];
        cur_node = inst == 'L' ? cur_node->left : cur_node->right;
        nb++;
    }

    network_free(n);

    return nb;
}

Node **starting_nodes_pt2(Network *n, size_t *nb_sn) {
    Node **starting_nodes = NULL;
    *nb_sn = 0;
    for(int i = 0; i < n->len; i++) {
        Node *node = n->nodes[i];
        char *label = node->label;
        if(label[2] == 'A') {
            *nb_sn = *nb_sn + 1;
            starting_nodes = realloc(starting_nodes, sizeof(Node*) * (*nb_sn));
            starting_nodes[*nb_sn - 1] = node;
        }
    }

    return starting_nodes;
}

uint64_t solve_part_2(char *filename) {
    Network *n = network_parse(filename);
    size_t nb_sn = 0;
    Node **starting_nodes = starting_nodes_pt2(n, &nb_sn);

    uint64_t *m_arr = NULL;
    size_t m_len = 0;

    for(int i = 0; i < nb_sn; i++) {
        Node *cur_node = starting_nodes[i];
        int nb = 0;
        while(cur_node->label[2] != 'Z') {
            char inst = n->instructions[nb % strlen(n->instructions)];
            cur_node = inst == 'L' ? cur_node->left : cur_node->right;
            nb++;
        }

        m_len++;
        m_arr = realloc(m_arr, sizeof(uint64_t) * m_len);
        m_arr[m_len - 1] = nb;
    }

    uint64_t l = lcm(m_arr, m_len);

    network_free(n);
    free(m_arr);
    free(starting_nodes);

    return l;
}

// https://stackoverflow.com/questions/19738919/gcd-function-for-c
uint64_t gcd(uint64_t a, uint64_t b) {
    int remainder = a % b;
    if (remainder == 0) {
        return b;
    }

    return gcd(b, remainder);
}

// https://www.geeksforgeeks.org/lcm-of-given-array-elements/
uint64_t lcm(uint64_t *m, size_t m_len) {
    uint64_t ans = m[0];
    for(int i = 1; i < m_len; i++) {
        ans = (m[i] * ans) / gcd(m[i], ans);
    }

    return ans;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        uint64_t result = solve_part_2("input.txt");
        printf("%lu\n", result);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
