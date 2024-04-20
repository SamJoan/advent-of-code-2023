import sys
import functools

RIGHT = 'RIGHT'
LEFT = 'LEFT'
UP = 'UP'
DOWN = 'DOWN'

DIRECTIONS = [RIGHT, LEFT, UP, DOWN]

DIR_CHANGE = {
    RIGHT: {
        '.': [RIGHT],
        '/': [UP],
        '\\': [DOWN],
        '-': [RIGHT],
        '|': [UP, DOWN],
    },
    LEFT: {
        '.': [LEFT],
        '/': [DOWN],
        '\\': [UP],
        '-': [LEFT],
        '|': [UP, DOWN],
    },
    UP: {
        '.': [UP],
        '/': [RIGHT],
        '\\': [LEFT],
        '-': [LEFT, RIGHT],
        '|': [UP],
    },
    DOWN: {
        '.': [DOWN],
        '/': [LEFT],
        '\\': [RIGHT],
        '-': [LEFT, RIGHT],
        '|': [DOWN],
    },
}

def parse_map(filename):
    fh = open(filename)
    map = []

    for y, line in enumerate(fh):
        line = line.strip()
        row = list(line)
        map.append(row)

    return map

def init_energised(map):
    energised = {}
    for y in range(len(map)):
        energised[y] = {}
        for x in range(len(map[0])):
            energised[y][x] = False

    return energised

def init_av(map):
    av = {}
    for y in range(len(map)):
        av[y] = {}
        for x in range(len(map[0])):
            av[y][x] = []

    return av

def pe(energised):
    energised = dict(energised)
    for y in energised:
        row = dict(energised[y])
        str = ""
        for x in row:
            val = row[x]
            if val:
                str += '#'
            else:
                str += '.'

        print(str)

def nav_next(map, energised, av, x, y, dir):
    if x == -1 or y == -1:
        return

    try:
        cur = map[y][x]
    except IndexError:
        return

    energised[y][x] = True
    if dir in av[y][x]:
        return
    else:
        av[y][x].append(dir)

    next_dirs = DIR_CHANGE[dir][cur]

    for next_dir in next_dirs:
        if next_dir == RIGHT:
            nav_next(map, energised, av, x + 1, y, next_dir)
        elif next_dir == UP:
            nav_next(map, energised, av, x, y - 1, next_dir) 
        elif next_dir == LEFT:
            nav_next(map, energised, av, x - 1, y, next_dir)
        elif next_dir == DOWN:
            nav_next(map, energised, av, x, y + 1, next_dir)

def count_energised(energised):
    count = 0
    for y in energised:
	for x in energised[y]:
	    if energised[y][x]:
		count += 1

    return count

def init_all(map):
    energised = init_energised(map)
    av = init_av(map)

    return energised, av


sys.setrecursionlimit(10000)

# map = parse_map('input_test.txt')
map = parse_map('input.txt')
max_nb = 0
for direction in DIRECTIONS:
    if direction == RIGHT:
        for y in range(len(map)):
            energised, av = init_all(map)
            nav_next(map, energised, av, 0, y, RIGHT)
            count = count_energised(energised) 
            max_nb = count if count > max_nb else max_nb
    elif direction == LEFT:
        for y in range(len(map)):
            energised, av = init_all(map)
            nav_next(map, energised, av, len(map[0]) - 1, y, LEFT)
            count = count_energised(energised) 
            max_nb = count if count > max_nb else max_nb
    elif direction == DOWN:
        for x in range(len(map[0])):
            energised, av = init_all(map)
            nav_next(map, energised, av, x, 0, DOWN)
            count = count_energised(energised) 
            max_nb = count if count > max_nb else max_nb
    elif direction == UP:
        for x in range(len(map[0])):
            energised, av = init_all(map)
            nav_next(map, energised, av, x, len(map) - 1, UP)
            count = count_energised(energised) 
            max_nb = count if count > max_nb else max_nb

print(max_nb)
