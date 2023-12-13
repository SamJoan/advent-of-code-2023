from pprint import pprint
from itertools import product
import functools
import sys

def get_springs(filename):
    fh = open(filename)
    rows = []
    for line in fh:
        line = line.strip()
        springs, damaged_spring_groups = line.split(' ')
        dsg = tuple([int(x) for x in damaged_spring_groups.split(',')])
        rows.append((tuple(springs), dsg))

    return rows

def parse_spring_groups(springs):
    result = []
    damaged_group_size = 0
    for c in springs:
        if c == '#':
            damaged_group_size += 1
        else:
            if damaged_group_size != 0:
                result.append(damaged_group_size)
                damaged_group_size = 0

    if damaged_group_size > 0:
        result.append(damaged_group_size)

    return result

# drew inspiration from https://github.com/CalSimmon/advent-of-code/blob/main/2023/day_12/solution.py
@functools.cache
def count_valid_possibilities(springs, dsg, group_size=0):
    if not springs:
        all_consumed = not dsg and not group_size
        can_consume_last = len(dsg) == 1 and dsg[0] == group_size
        return all_consumed or can_consume_last

    symbols = ['#', '.'] if springs[0] == '?' else springs[0]
    valid_posibilities = 0
    for s in symbols:
        if s == '#':
            valid_posibilities += count_valid_possibilities(springs[1:], dsg, group_size+1)
        if s == '.':
            if group_size != 0:
                if dsg and dsg[0] == group_size:
                    valid_posibilities += count_valid_possibilities(springs[1:], dsg[1:])
            else:
                valid_posibilities += count_valid_possibilities(springs[1:], dsg)
    
    return valid_posibilities

def unfold(springs):
    unfolded_springs = []
    for springs, damaged_spring_groups in springs:
        unfolded_spring = tuple('?'.join([''.join(springs)]*5))
        unfolded_dsg = damaged_spring_groups*5
        unfolded_springs.append((unfolded_spring, unfolded_dsg))

    return unfolded_springs

# springs = unfold(get_springs('input_test.txt'))
springs = unfold(get_springs('input.txt'))
# springs = get_springs('input_test.txt')
# springs = get_springs('input.txt')

total = 0
for springs, damaged_spring_groups in springs:
    total += count_valid_possibilities(springs, damaged_spring_groups)

print(total)
