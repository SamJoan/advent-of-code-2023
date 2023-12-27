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

def next_valid(map, pos, steps, directions, already_visited):
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

def get_nodes(map, start, end):
    q = Queue()
    q.put((0, start, start, set()))
    paths = []
    nodes = {}
    nodes[start] = set()
    while q:
        try:
            tpl = q.get(block=False)
        except Empty:
            break

        steps, pos, parent_node, already_visited = tpl
        x, y = pos

        if pos in already_visited:
            continue

        already_visited.add(pos)

        if pos == end:
            nodes[parent_node].add((x, y, steps))

	directions = DIRECTIONS

        valid_next = next_valid(map, pos, steps, directions, already_visited)
        for next_tpl in valid_next:
            next_x, next_y = next_tpl[1]

            if len(valid_next) > 2:
                nodes[parent_node].add((x, y, steps))
                new_parent_node = (x, y)
                if not new_parent_node in nodes:
                    nodes[new_parent_node] = set()
                    nodes[new_parent_node].add((parent_node[0], parent_node[1], steps))

                # print(pos, len(valid_next), valid_next)
                # pm(map, pos)
                # print(nodes)
                # raw_input()
                av = set(already_visited)
                q.put((1, next_tpl[1], new_parent_node, av))
            else:
                q.put((next_tpl[0], next_tpl[1], parent_node, already_visited))
        
    return nodes

def max_len_hike(nodes, start, end):
    q = Queue()

    q.put((0, start, set()))

    print(end)

    max_steps = 0
    while q:
        try:
            steps, node, already_visited = q.get(block=False)
        except Empty:
            break

        if node in already_visited:
            continue
        else:
            already_visited.add(node)

        if node == end:
            print('ended on', steps)
            if steps > max_steps:
                max_steps = steps
            continue

        vertices = nodes[node]
        for vertex in vertices:
            x, y, vertex_steps = vertex
            new_av = set(already_visited)
            q.put((steps + vertex_steps, (x, y), new_av))

    return max_steps

# map, start, end = parse_map("input_test.txt")
map, start, end = parse_map("input.txt")
nodes = get_nodes(map, start, end)
max_len_hike = max_len_hike(nodes, start, end)

print(max_len_hike)
