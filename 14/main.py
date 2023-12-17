import sys
from pprint import pprint

def parse_platform(filename):
    fh = open(filename)
    platform = []
    for line in fh:
        line = line.strip()
        platform.append(list(line))

    return platform

def max_pos_north(p, x, y):
    while True:
        y = y - 1
        next_c = p[y][x]
        if y == -1 or next_c != '.':
            return y + 1

def tilt_platform_north(p):
    for y in range(1, len(p)):
        for x, c in enumerate(p[y]):
            if c == 'O':
                max_y = max_pos_north(p, x, y)
                if max_y != y:
                    p[max_y][x] = 'O'
                    p[y][x] = '.'

    return p

def min_pos_west(p, x, y):
    while True:
        x = x - 1
        if x == -1 or p[y][x] != '.':
            return x + 1

def tilt_platform_west(p):
    for y, _ in enumerate(p):
        for x, c in enumerate(p[y]):
            if x > 0:
                if c == 'O':
                    min_x = min_pos_west(p, x, y)
                    if min_x != x:
                        p[y][min_x] = 'O'
                        p[y][x] = '.'

    return p

def max_y_south(p, x, y):
    while True:
        y = y + 1
        try:
            if p[y][x] != '.':
                return y - 1
        except IndexError:
            return y - 1

def tilt_platform_south(p):
    for y, _ in reversed(list(enumerate(p))):
        for x, c in enumerate(p[y]):
            if y < len(p) - 1:
                if c == 'O':
                    max_y = max_y_south(p, x, y)
                    if max_y != y:
                        p[max_y][x] = 'O'
                        p[y][x] = '.'

    return p

def max_x_east(p, x, y):
    while True:
        x = x + 1
        try:
            if p[y][x] != '.':
                return x - 1
        except IndexError:
            return x - 1

def tilt_platform_east(p):
    for y, _ in enumerate(p):
        for x, c in reversed(list(enumerate(p[y]))):
            if x < len(p[y]) - 1:
                if c == 'O':
                    max_x = max_x_east(p, x, y)
                    if max_x != x:
                        p[y][max_x] = 'O'
                        p[y][x] = '.'

    return p

def tilt_platform_cycle(p):
    p = tilt_platform_north(p)
    p = tilt_platform_west(p)
    p = tilt_platform_south(p)
    p = tilt_platform_east(p)

    return p

def pp(p):
    for line in p:
        print ''.join(line)

def calculate_weight(p):
    rock_weight_total = 0
    for nb, line in enumerate(p):
        weight = len(p) - nb
        rock_weight = line.count('O') * weight
        rock_weight_total += rock_weight

    return rock_weight_total

# p = parse_platform('input_test.txt')
p = parse_platform('input.txt')

cycles = 0
for _ in range(1000):
    p = tilt_platform_cycle(p)
    cycles += 1


results = []
for nb in range(1000):
    p = tilt_platform_cycle(p)
    rock_weight = calculate_weight(p)
    results.append(rock_weight)
    cycles += 1

pattern_beginning = results[0:50]
cycle_beginnings = []
for nb, result in enumerate(results):
    all_equal = True
    for x in range(len(pattern_beginning)):
        try:
            if results[x] !=  results[x + nb]:
                all_equal = False
        except IndexError:
            all_equal = False
            break

    if all_equal:
        cycle_beginnings.append(nb)

cycle_length = -1
for i, c in enumerate(cycle_beginnings):
    if i > 0:
        prev_cycle_length = cycle_length
        cycle_length = cycle_beginnings[i] - cycle_beginnings[i - 1]
        assert cycle_length == prev_cycle_length or prev_cycle_length == -1

cycle = []
for _ in range(cycle_length):
    p = tilt_platform_cycle(p)
    rock_weight = calculate_weight(p)
    cycle.append(rock_weight)
    cycles += 1

print(cycle)
desired_count = 1000000000
remaining_cycles = desired_count - cycles - 1 # off by one.

print cycle[remaining_cycles % cycle_length]
