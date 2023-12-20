import sys
import copy
from queue import PriorityQueue

HORIZ = 'HORIZ'
VERT = 'VERT'

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


# My original solution was too inefficient to finish because I was not using a priority queue. :(
# Inspiration from https://cutonbuminband.github.io/AOC/qmd/2023.html#day-17-clumsy-crucible
def nav(map, minval=1, maxval=3):
    q = PriorityQueue()
    q.put((0, (0, 0, HORIZ)))
    q.put((0, (0, 0, VERT)))
    seen = set()
    max_x = len(map[0]) - 1
    max_y = len(map) - 1
    
    while q:
        cost, vec = q.get()
        x, y, dir = vec

        if x == max_x and y == max_y:
            break

        if (x, y, dir) in seen:
            continue
        seen.add((x, y, dir))
        
        original_cost = cost
        for s in [1, -1]:
            cost = original_cost
            new_x, new_y = x, y
            for i in range(1, maxval + 1):
                if dir == HORIZ:
                    new_x = x + i * s
                if dir == VERT:
                    new_y = y + i * s

                if new_x < 0 or new_x > max_x or new_y < 0 or new_y > max_y:
                    break

                cost += map[new_y][new_x]
                new_dir = HORIZ if dir == VERT else VERT

                new_vec = (new_x, new_y, new_dir) 
                if new_vec in seen:
                    continue

                if i >= minval:
                    q.put((cost, new_vec))

    return cost

# map = parse_map("input_test.txt")
map = parse_map("input.txt")

# min = nav(map)
min = nav(map, minval=4, maxval=10)
print(min)
