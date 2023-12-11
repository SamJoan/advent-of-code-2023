from pprint import pprint
from itertools import combinations
import sys

def get_universe(filename):
    universe = []
    fh = open(filename)
    for line in fh:
        line = line.strip()
        universe.append(list(line))
    
    return universe

def expand_universe(universe, years):

    expanded_universe = []
    for row in universe:
        all_empty = True
        for c in row:
            if c == '#':
                all_empty = False

        if all_empty:
            for _ in range(years -1):
                expanded_universe.append(row)

        expanded_universe.append(row)


    for col in range(len(expanded_universe[0]) -1, -1, -1):
        all_empty = True
        for row in expanded_universe:
            c = row[col]
            if c == '#':
                all_empty = False

        if all_empty:
            for row in universe:
                for _ in range(years -1):
                    row.insert(col, '.')
                pass

    return expanded_universe

def get_galaxies(universe):
    galaxies = []
    for y, row in enumerate(universe):
        for x, c in enumerate(row):
            if c == '#':
                galaxies.append((x, y))

    return galaxies

def calculate_distances(galaxy_combos):
    total_distance = 0
    for xy_a, xy_b in galaxy_combos:
        x_a, y_a = xy_a
        x_b, y_b = xy_b
        distance = abs(x_a - x_b) + abs(y_a - y_b)
        total_distance += distance

    return total_distance


last_result = None
for years in [1, 2]:
    # smol_universe = get_universe('input_test.txt')
    smol_universe = get_universe('input.txt')
    universe = expand_universe(smol_universe, years)

    galaxies = get_galaxies(universe)
    galaxy_combos = list(combinations(galaxies, 2))
    total_distance = calculate_distances(galaxy_combos)

    if last_result:
        delta = total_distance - last_result
    else:
        base_total_distance = total_distance
        last_result = total_distance


years = 1000000
new_total = base_total_distance + (delta * (years-1))

print(new_total)





# for row in universe:
    # print(''.join(row))

