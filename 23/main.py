import sys
from Queue import PriorityQueue, Empty, Queue
from pprint import pprint

UP = 'UP'
DOWN = 'DOWN'
LEFT = 'LEFT'
RIGHT = 'RIGHT'

DIRECTIONS = [UP, DOWN, LEFT, RIGHT]
SLOPES = {
    '^': UP,
    '>': RIGHT,
    '<': LEFT,
    'v': DOWN
}

def parse_map(filename):
    fh = open(filename)
    map = []
    for line in fh:
        line = line.strip()
        map.append(list(line))

    last_row = len(map) - 1
    start = (map[0].index('.'), 0)
    end = (map[last_row].index('.'), last_row)

    return map, start, end

def pm(map, pos):
    for y, _ in enumerate(map):
        str = ""
        for x, c in enumerate(map[y]):
            if (x,y) == pos:
                str += "X"
            else:
                str += c

        print(str)

def next_valid(map, pos, steps, directions):
    max_y = len(map) - 1
    max_x = len(map[0]) - 1

    x, y = pos
    valid_next_tpl = []
    for dir in directions:
        next_x = x
        next_y = y

        if dir == UP:
            next_y -= 1
        elif dir == DOWN:
            next_y += 1
        elif dir == LEFT:
            next_x -= 1
        elif dir == RIGHT:
            next_x += 1

        if next_x > max_x or next_y > max_y or next_y < 0 or next_x < 0:
            continue

        next_c = map[next_y][next_x]
        next_steps = steps + 1
        if next_c == '#':
            continue

        next_tpl = (next_steps, (next_x, next_y))
        valid_next_tpl.append(next_tpl)

    return valid_next_tpl

def take_a_hike(map, start, end):
    q = Queue()
    q.put((0, start, set()))
    paths = []
    while q:
        try:
            tpl = q.get(block=False)
        except Empty:
            print("Exiting because queue empty.")
            break

        steps, pos, already_visited = tpl
        x, y = pos

        if pos in already_visited:
            continue
        already_visited.add(pos)

        if pos == end:
            paths.append(steps)

        c = map[y][x]
        if c in SLOPES:
            directions = [SLOPES[c]]
        else:
            directions = DIRECTIONS

        valid_next = next_valid(map, pos, steps, directions)
        for next_tpl in valid_next:
            next_x, next_y = next_tpl
            if not len(valid_next) == 1:
                already_visited = set(already_visited)

            if not (next_x, next_y) in already_visited:
                q.put((next_tpl[0], next_tpl[1], already_visited))
        
    return max(paths)

# map, start, end = parse_map("input_test.txt")
map, start, end = parse_map("input.txt")
longest_hike = take_a_hike(map, start, end)
print(longest_hike)
