
struct Number {
    long int number_value;
    int start_pos;
    int end_pos;
};

long int find_part_numbers(char *prev_line, char *cur_line_in, char *next_line);
long int find_gears(char *prev_line, char *cur_line_in, char *next_line);
struct Number *get_number(char *line, int pos);

