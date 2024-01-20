
int parse_ints(char *numbers_in, int **numbers_ptr) {
    char *numbers_str = strdup(numbers_in);
    char *numbers_str_orig = numbers_str;
    
    int *numbers = NULL;
    char *token = NULL;
    int i = 0;
    while((token = strtok_r(numbers_str, " ", &numbers_str))) {
        numbers = realloc(numbers, (i + 1) * sizeof(int));

        sscanf(token, "%d", &numbers[i]);

        i++;
    }

    free(numbers_str_orig);
    *numbers_ptr = numbers;

    return i;
}

