import sys
from pprint import pprint

def parse_map(filename):
    fh = open(filename)
    world = {}
    for nb, line in enumerate(fh):
        line = line.strip()
        if line == '':
            continue

        if nb == 0:
            instructions = line
        else:
            location, left_right = line.split('=')
            left, right = [x.strip("() ") for x in left_right.split(',')]
            location = location.strip()
            world[location] = (left, right)

    return instructions, world


# instructions, world = parse_map("input_test.txt")
instructions, world = parse_map("input.txt")

cur_pos = "AAA"
nb = 0
while True:
    instruction = instructions[nb % len(instructions)]
    if instruction == "L":
        cur_pos = world[cur_pos][0]
    else:
        cur_pos = world[cur_pos][1]

    if cur_pos == "ZZZ":
        print nb + 1
        break

    nb += 1

