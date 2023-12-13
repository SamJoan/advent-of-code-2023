from pprint import pprint
from functools import cache
from itertools import product
import sys

def listToTuple(function):
    def wrapper(*args):
        args = [tuple(x) if type(x) == list else x for x in args]
        result = function(*args)
        result = tuple(result) if type(result) == list else result
        return result
    return wrapper

def get_springs(filename):
    fh = open(filename)
    rows = []
    for line in fh:
        line = line.strip()
        springs, damaged_spring_groups = line.split(' ')
        rows.append((list(springs), damaged_spring_groups))

    return rows

def parse_spring_groups(springs):
    result = []
    damaged_group_size = 0
    for c in springs:
        if c == '#':
            damaged_group_size += 1
        else:
            if damaged_group_size != 0:
                result.append(str(damaged_group_size))
                damaged_group_size = 0

    if damaged_group_size > 0:
        result.append(str(damaged_group_size))

    result_str = ','.join(result)
    return result_str

def determine_valid(springs, damaged_spring_groups):
    result_str = parse_spring_groups(springs)

    return result_str == damaged_spring_groups

@listToTuple
@cache
def count_valid_possibilities(springs, damaged_spring_groups):
    try:
        qmark_pos = springs.index('?')
    except ValueError:
        qmark_pos = -1

    if qmark_pos != -1:
        valid_combinations = 0

        new_springs = list(springs)
        new_springs[qmark_pos] = '.'
        valid_combinations += count_valid_possibilities(new_springs, damaged_spring_groups)

        new_springs = list(springs)
        new_springs[qmark_pos] = '#'
        valid_combinations += count_valid_possibilities(new_springs, damaged_spring_groups)

        return valid_combinations
    else:
        return parse_spring_groups(springs) == damaged_spring_groups

def unfold(springs):
    unfolded_springs = []
    for springs, damaged_spring_groups in springs:
        unfolded_spring = list('?'.join([''.join(springs)]*5))
        unfolded_dsg = ','.join([damaged_spring_groups]*5)
        print(unfolded_dsg)
        unfolded_springs.append((unfolded_spring, unfolded_dsg))

    return unfolded_springs

springs = unfold(get_springs('input_test.txt'))
# springs = get_springs('input_test.txt')
# springs = get_springs('input.txt')

total = 0
for springs, damaged_spring_groups in springs:
    total += count_valid_possibilities(springs, damaged_spring_groups)

print(total)
