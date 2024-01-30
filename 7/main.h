#include <stdint.h>
#include <stdbool.h>

enum HandType {HIGH, PAIR, TWO_PAIR, THREE, FULL, FOUR, FIVE};

typedef struct {
    char *cards;
    uint64_t bid;
    enum HandType type;
} Hand;

typedef struct {
    Hand **data;
    int len;
} Hands;

Hand *hand_init(char *cards, uint64_t bid, bool jokers);
Hands *hands_init();
Hands *parse_hands(const char *filename, bool jokers);
int hand_compare(const void *hand_a_in, const void *hand_b_in, void *jokers_in);
int individual_card_compare(char *cards_a, char *cards_b, bool jokers);
uint64_t solve_part_1(const char *filename, bool jokers);
uint64_t solve_part_2(char *filename);
void hands_add(Hands *hands, Hand *hand);
void hands_free(Hands *hands);
void hands_sort(Hands *hands, bool jokers);
