import sys
from pprint import pprint

def up(coords):
    x, y = coords
    y -= 1
    return x, y

def down(coords):
    x, y = coords
    y += 1
    return x, y

def left(coords):
    x, y = coords
    x -= 1
    return x, y

def right(coords):
    x, y = coords
    x += 1
    return x, y


directions = {
    "up": up,
    "down": down,
    "left": left,
    "right": right
}

opposite_directions = {
    "up": "down",
    "down": "up",
    "left": "right",
    "right": "left"
}

pipes = {
    '|': ("up", "down"),
    "L": ("up", "right"),
    "J": ("up", "left"),
    "-": ("left", "right"),
    "7": ("left", "down"),
    "F": ("right", "down")
}

def parse_map(filename):
    map = []
    fh = open(filename)
    y = 0
    for line in fh:
        line = line.strip()
        row = []
        x = 0
        for c in line:
            row.append(c)
            if c == "S":
                starting_coords = (x, y)

            x += 1

        map.append(row) 
        y += 1

    return map, starting_coords

def get_char(map, coords):
    x, y = coords
    return map[y][x]

def connects(pipe, direction):
    try:
        p = pipes[pipe]
        return direction in p
    except KeyError:
        return False

def get_likely_directions(map, starting_coords):
    c_up = get_char(map, up(starting_coords))
    c_down = get_char(map, down(starting_coords))
    c_left = get_char(map, left(starting_coords))
    c_right = get_char(map, right(starting_coords))

    likely_directions = []
    if connects(c_up, "down"):
        likely_directions.append("up")
    if connects(c_down, "up"):
        likely_directions.append("down")
    if connects(c_left, "right"):
        likely_directions.append("left")
    if connects(c_right, "left"):
        likely_directions.append("right")

    for d in pipes:
        p = pipes[d]
        x, y = starting_coords
        if likely_directions[0] in p and likely_directions[1] in p:
            map[y][x] = d
            break

    return likely_directions


def navigate_map(map, starting_coords):
    likely_directions = get_likely_directions(map, starting_coords)
    step_coords = [starting_coords]
    for direction in likely_directions:
        direction_method = directions[direction]
        next_coords = direction_method(starting_coords)
        step_coords.append(next_coords)
        next_char = get_char(map, next_coords)
        next_direction = direction
        steps = 1
        while True:
            if next_coords == starting_coords:
                break

            cur_coords = next_coords
            cur_char = next_char
            cur_direction = next_direction

            p = pipes[cur_char]
            opposite_direction = opposite_directions[cur_direction]
            next_direction = p[1 - p.index(opposite_direction)]

            direction_method = directions[next_direction]
            next_coords = direction_method(cur_coords)
            step_coords.append(next_coords)
            next_char = get_char(map, next_coords)

            steps += 1

    return steps / 2, step_coords

def count_step_coords(map, step_coords):
    rows = []
    for y in range(len(map)):
        row = []
        for x in range(len(map[0])):
            coords = (x, y)
            if coords in step_coords:
		row.append(get_char(map, coords))
            else:
                row.append(' ')

        rows.append(row)

    for y, row in enumerate(rows):
        # https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
        nb_touches = 0
        for x, c in enumerate(row):
            if c != ' ':
                p = pipes[c]
                if 'up' in p: # this line of code I got from a spoiler. I *think* it avoids gt
                    rows[y][x] = "T"
                    nb_touches += 1
            else:
                is_inside = nb_touches % 2 == 1
                if is_inside:
                    rows[y][x] = "I"

    c = 0
    for row in rows:
        c += row.count('I')
        print(''.join(row))
    
    return c

# map, starting_coords = parse_map('input_test.txt')
# map, starting_coords = parse_map('input_test_complex.txt')
# map, starting_coords = parse_map('input_test_area.txt')
map, starting_coords = parse_map('input_test_area_complex.txt')
# map, starting_coords = parse_map('input.txt')
steps, step_coords = navigate_map(map, starting_coords)
c = count_step_coords(map, step_coords)

print(c)


