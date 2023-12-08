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

starting_positions = []
for k in world:
    if k.endswith("A"):
        starting_positions.append(k)

nb = 0
cur_pos = list(starting_positions)
first_reached_z = [-1] * len(cur_pos)
while True:
    instruction = instructions[nb % len(instructions)]
    nb_finished = 0
    for pos, starting_position in enumerate(starting_positions):
        if instruction == "L":
            cur_pos[pos] = world[cur_pos[pos]][0]
        else:
            cur_pos[pos] = world[cur_pos[pos]][1]

        if cur_pos[pos].endswith('Z'):
            print("%s reached pos %s" %( pos, cur_pos[pos]))
            first_reached_z[pos] = nb + 1

    nb_reached_z = 0
    for x in first_reached_z:
        if x != -1:
            nb_reached_z += 1
    
    if nb_reached_z == len(cur_pos):
        print(first_reached_z)
        print("Grab those numbers and paste them here: https://www.calculatorsoup.com/calculators/math/lcm.php")
        sys.exit()

    nb += 1

