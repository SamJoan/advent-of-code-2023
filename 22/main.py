import sys
from pprint import pprint
import uuid

X = 'X'
Y = 'Y'
Z = 'Z'

ID = -1

class Block():
    def __init__(self, start, end):
        global ID
        ID += 1

        self.id = ID
        self.start = start
        self.end = end

    def find_varying_coord(self):
        if self.start[0] != self.end[0]:
            return X
        elif self.start[1] != self.end[1]:
            return Y
        elif self.start[2] != self.end[2]:
            return Z
        else:
            return None

    def holding_me(self, new_space):
        s, e = self.start, self.end
        z = s[2] - 1
        holding = set()
        if z == 0:
            return holding

        var_coord = self.find_varying_coord()
        if var_coord == X:
            for x in range(s[0], e[0] + 1):
                if new_space[z][s[1]][x] != '.':
                    holding.add(new_space[z][s[1]][x])
        elif var_coord == Y:
            for y in range(s[1], e[1] + 1):
                if new_space[z][y][s[0]] != '.':
                    holding.add(new_space[z][y][s[0]])
        elif var_coord == None:
            if new_space[z][s[1]][s[0]] != '.':
                holding.add(new_space[z][s[1]][s[0]])
        elif var_coord == Z:
            if new_space[z][s[1]][s[0]] != '.':
                holding.add(new_space[z][s[1]][s[0]])
        
        return holding

    def holding(self, new_space):
        s, e = self.start, self.end
        z = e[2] + 1
        holding = set()
        if z == 0:
            return holding

        var_coord = self.find_varying_coord()
        if var_coord == X:
            for x in range(s[0], e[0] + 1):
                if new_space[z][s[1]][x] != '.':
                    holding.add(new_space[z][s[1]][x])
        elif var_coord == Y:
            for y in range(s[1], e[1] + 1):
                if new_space[z][y][s[0]] != '.':
                    holding.add(new_space[z][y][s[0]])
        elif var_coord == None:
            if new_space[z][s[1]][s[0]] != '.':
                holding.add(new_space[z][s[1]][s[0]])
        elif var_coord == Z:
            if new_space[z][s[1]][s[0]] != '.':
                holding.add(new_space[z][s[1]][s[0]])
        
        return holding

def parse_blocks(filename):
    fh = open(filename)
    blocks = []
    for line in fh:
        line = line.strip()
        start_str, end_str = line.split('~')
        start = tuple([int(x) for x in start_str.split(',')])
        end = tuple([int(x) for x in end_str.split(',')])
        blocks.append(Block(start, end))

    global ID
    ID = -1

    return blocks

def create_3d_space(blocks):
    max_x, max_y, max_z = 0, 0, 0
    for block in blocks:
        start, end = block.start, block.end
        x, y, z = start
        max_x = x + 1 if x + 1 > max_x else max_x
        max_y = y + 1 if y + 1 > max_y else max_y
        max_z = z + 1 if z + 1 > max_z else max_z
        x, y, z = end
        max_x = x + 1 if x + 1 > max_x else max_x
        max_y = y + 1 if y + 1 > max_y else max_y
        max_z = z + 1 if z + 1 > max_z else max_z

    space = {}
    for z in range(max_z):
        space[z] = {}
        for y in range(max_y):
            space[z][y] = {}
            for x in range(max_x):
                space[z][y][x] = '.'

    return space

def populate_block(space, block):
    var_coord = block.find_varying_coord()
    s, e = block.start, block.end
    if var_coord == X:
        for x in range(s[0], e[0] + 1):
            space[s[2]][s[1]][x] = block
    elif var_coord == Y:
        for y in range(s[1], e[1] + 1):
            space[s[2]][y][s[0]] = block
    elif var_coord == Z:
        for z in range(s[2], e[2] + 1):
            space[z][s[1]][s[0]] = block
    elif var_coord == None:
        space[s[2]][s[1]][s[0]] = block

    return space

def populate(space, blocks):
    for block in blocks:
        space = populate_block(space, block)

    return space

def ps_x(space):
    for z in range(len(space)-1, 0-1, -1):
        s = str(z) + " "
        for x in range(len(space[0][0])):
            all_empty = True
            for y in range(len(space[0])):
                c = space[z][y][x]
                if c != '.':
                    all_empty = False
            
            if all_empty:
                s += '.'
            else:
                s += '#'

        print(s)

def get_new_z(new_space, block):
    z = block.start[2]
    s, e = block.start, block.end

    original_start_z = s[2]
    original_end_z = e[2]

    while True:
        var_coord = block.find_varying_coord()

        z = z - 1
        if z == 0:
            return z + 1, z + 1
        if var_coord == X:
            for x in range(s[0], e[0] + 1):
                if new_space[z][s[1]][x] != '.':
                    return z + 1, z + 1
        elif var_coord == Y:
            for y in range(s[1], e[1] + 1):
                if new_space[z][y][s[0]] != '.':
                    return z + 1, z + 1
        elif var_coord == None:
            if new_space[z][s[1]][s[0]] != '.':
                return z + 1, z + 1
        elif var_coord == Z:
            if new_space[z][s[1]][s[0]] != '.':
                return z + 1, z + 1 + (original_end_z - original_start_z)

def apply_gravity(space):
    new_space = create_3d_space(blocks)
    new_blocks = []
    already_dropped = set()
    for z in space:
        for y in space[z]:
            for x in space[z][y]:
                c = space[z][y][x]
                if c != '.':
                    block = c
                    if block.id in already_dropped:
                        continue

                    start, end = block.start, block.end
                    new_z_start, new_z_end = get_new_z(new_space, block)

                    new_start = (start[0], start[1], new_z_start)
                    new_end = (end[0], end[1], new_z_end)
                    new_block = Block(new_start, new_end)

                    populate_block(new_space, new_block)
                    new_blocks.append(new_block)
                    already_dropped.add(block.id)

    return new_space, new_blocks

def count_disintegrable(new_space, blocks):
    disintegrable = 0
    for block in blocks:
        holding = block.holding(new_space)
        if len(holding) == 0:
            disintegrable += 1
            continue

        all_held_by_another = True
        for block_above in holding:
            if len(block_above.holding_me(new_space)) == 1:
                all_held_by_another = False

        if all_held_by_another:
            disintegrable += 1

        print(disintegrable, block.id, len(holding), all_held_by_another)

    return disintegrable
        
# blocks = parse_blocks("input_test.txt")
blocks = parse_blocks("input.txt")
space = create_3d_space(blocks)
space = populate(space, blocks)

space, blocks = apply_gravity(space)
nb = count_disintegrable(space, blocks)
print(nb)


