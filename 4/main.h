
typedef struct {
    int *winning_numbers;
    int winning_numbers_size;
    int *numbers;
    int numbers_size;
    int id;
    int copies;
} Card;

int calculate_score(Card *card);
int solve_part_1(char *filename);
int solve_part_2(char *filename);
void free_card(Card *card);
void parse_card(Card *card, const char *card_str_in);
