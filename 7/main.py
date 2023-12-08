from collections import Counter
from pprint import pprint
import sys

SORT_ORDER = {"A": "A", "K": "B", "Q": "C", "T": "E", "9": "F", "8": "G", "7": "H", "6": "I", "5": "J", "4": "K", "3": "L", "2": "M", "J": "N"}

def sort_key(input):
    sort_key = ""
    cards = input[0]
    for c in cards:
        sort_key += SORT_ORDER[c]

    return sort_key

def convert_jokers(counter):
    nb_jokers = counter['J']
    if nb_jokers > 0 and nb_jokers != 5:
        del counter['J']
        mc = counter.most_common(5)
        print(counter)
        counter[mc[0][0]] += nb_jokers

    return counter

def parse_cards(filename):
    fh = open(filename)
    
    buckets = {
        'five': [],
        'four': [],
        'full': [],
        'three': [],
        'twopair': [],
        'onepair': [],
        'highcard': []
    }

    for line in fh:
        line = line.strip()
        if line == '':
            continue

        cards, bid = line.split(' ')
        counter = Counter(cards)

        counter = convert_jokers(counter)
        mc = counter.most_common(5)

        first_card, count = mc[0]
        if count == 5:
            buckets['five'].append((cards, bid))
        elif count == 4:
            buckets['four'].append((cards, bid))
        elif count == 3:
            second_card, second_count = mc[1]
            if second_count == 2:
                buckets['full'].append((cards, bid))
            else:
                buckets['three'].append((cards, bid))
        elif count == 2:
            second_card, second_count = mc[1]
            if second_count == 2:
                buckets['twopair'].append((cards, bid))
            else:
                buckets['onepair'].append((cards, bid))
        else:
            buckets['highcard'].append((cards, bid))


    for key in buckets:
        buckets[key].sort(key=sort_key, reverse=True)

    return buckets

# buckets = parse_cards("input_test_mine.txt")
# buckets = parse_cards("input_test.txt")
buckets = parse_cards("input.txt")
total = 0

rank = 1
for card, bid in buckets['highcard'] + buckets['onepair'] + buckets['twopair'] + buckets['three'] + buckets['full'] + buckets['four'] + buckets['five']:
    round_winnings = int(bid) * rank
    total += round_winnings
    rank += 1

print(total)
# cards = parse_cards("input_test.txt")

