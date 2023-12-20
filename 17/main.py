import sys
import copy
import functools

UP = 'UP' 
DOWN = 'DOWN'
LEFT = 'LEFT'
RIGHT = 'RIGHT'

DIR_CHANGE = {
    RIGHT: [RIGHT, DOWN, UP],
    DOWN: [DOWN, RIGHT, LEFT],
    UP: [RIGHT, LEFT, UP],
    LEFT: [DOWN, UP, LEFT],
}

def convert(l):
    return tuple(convert(x) for x in l) if type(l) is list else l

def parse_map(filename):
    fh = open(filename)
    map = []
    for line in fh:
        line = line.strip()
        map.append(list([int(x) for x in line]))

    return convert(map)

def get_next_pos(x, y, dir):
    if dir == RIGHT:
        x += 1
    elif dir == LEFT:
        x -= 1
    elif dir == UP:
        y -= 1
    elif dir == DOWN:
        y += 1

    return x, y

class CantNavigate(Exception):
    pass

def init_av(map):
    av = {}
    for y in range(len(map)):
        av[y] = {}
        for x in range(len(map[0])):
            av[y][x] = []

    return av

def pv(map, visited):
    for y in range(len(map)):
        res = ""
        for x in range(len(map[0])):
            if (x, y, DOWN) in visited:
                res += "v"
            elif (x, y, UP) in visited:
                res += '^'
            elif (x, y, LEFT) in visited:
                res += '<'
            elif (x, y, RIGHT) in visited:
                res += ">"
            else:
                res += "."

        print(res)


def nav(map, x, y, conseq, cur_dir, visited=None):
    if conseq >= 3:
        raise CantNavigate()

    if conseq == -1:
        conseq = 0
        cur_heat_loss = 0
    else:
        if x == -1 or y == -1:
            raise CantNavigate()

        try:
            cur_heat_loss = map[y][x]
        except IndexError:
            raise CantNavigate()

    if visited == None:
        visited = set()
    else:
        # visited = copy.copy(visited)
        loc_tuple = (x, y, conseq, cur_dir)
        if not loc_tuple in visited:
            visited.add(loc_tuple)
        else:
            raise CantNavigate()

    # pv(map, visited)
    # raw_input()

    # print(x, y, conseq, cur_dir)
    # raw_input()
    target_x = len(map) - 1
    target_y = len(map[0]) - 1

    if x == len(map) -1 and y == len(map[0]) -1:
        return cur_heat_loss
    else:
        min_heat_loss = -1
        for dir in DIR_CHANGE[cur_dir]:
            next_x, next_y = get_next_pos(x, y, dir)
            next_conseq = 0 if dir != cur_dir else conseq + 1
            try:
                heat_loss = nav(map, next_x, next_y, next_conseq, dir, set(visited))
                if min_heat_loss != -1:
                    min_heat_loss = heat_loss if heat_loss < min_heat_loss else min_heat_loss
                else:
                    min_heat_loss = heat_loss
            except CantNavigate:
                continue
        
        return min_heat_loss + cur_heat_loss

sys.setrecursionlimit(10000)
map = parse_map("input_test.txt")

min = nav(map, 0, 0, -1, RIGHT)
print(min)
