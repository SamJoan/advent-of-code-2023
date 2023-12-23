import sys
import pprint

def parse_map(filename):
    fh = open(filename)
    map = []
    y = 0
    for line in fh:
        row = list(line.strip())
        if 'S' in line:
            start_pos = line.index('S'), y
        map.append(row)
        y += 1

    return map, start_pos

UP = 'UP'
DOWN = 'DOWN'
LEFT = 'LEFT'
RIGHT = 'RIGHT'

def move(cur_pos, direction):
    x, y = cur_pos
    if direction == UP:
        y -= 1
    elif direction == DOWN:
        y += 1
    elif direction == LEFT:
        x -= 1
    elif direction == RIGHT:
        x += 1

    return x, y


def take_step(map, cur_pos, remaining_steps, visited):
    max_x = len(map[0]) - 1
    max_y = len(map) - 1
    posrem = (cur_pos, remaining_steps)

    
    if visited == None:
        visited = set()

    if posrem in visited:
        return 0
    else:
        visited.add(posrem)

    if remaining_steps == 0:
        return 1 

    directions = [UP, DOWN, LEFT, RIGHT]
    total_positions = 0
    for direction in directions:
        next_x, next_y = move(cur_pos, direction)

	next_x_rem = next_x % len(map[0])
	next_y_rem = next_y % len(map)

        next_char = map[next_y_rem][next_x_rem]
        is_rock = next_char == '#'
        if is_rock:
            continue

        total_positions += take_step(map, (next_x, next_y), remaining_steps - 1, visited)

    return total_positions

map, start_pos = parse_map("input_test.txt")
# map, start_pos = parse_map("input.txt")

last_result = 0
for nb in range(6, 500):
    positions = take_step(map, start_pos, nb, None)
    print(nb, positions, positions - last_result)
    last_result = positions

print(positions)
