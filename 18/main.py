import sys
from pprint import pprint

def get_positions(filename):
    fh = open(filename)
    orig_pos = (0, 0)
    positions = [orig_pos]
    x, y = orig_pos
    for line in fh:
        line = line.strip()
        dir, distance, colour = line.split(' ')
        distance = int(distance)
        if dir == 'U':
            y -= distance
        elif dir == 'D':
            y += distance
        elif dir == 'L':
            x -= distance
        elif dir == 'R':
            x += distance

        if (x, y) == orig_pos:
            break

        positions.append((x, y))

    return positions

def get_area(positions):
    area = 0
    for nb, pos in enumerate(positions):
        x, y = pos
        try:
            next_x, next_y = positions[nb + 1]
        except IndexError:
            next_x, next_y = positions[0]

        shoelace = (x * next_y) + (y * next_x * -1)
        area += shoelace

    return area / 2

def get_perimeter(positions):
    perimeter = 0
    for nb, pos in enumerate(positions):
        x, y = pos
        try:
            next_x, next_y = positions[nb + 1]
        except IndexError:
            next_x, next_y = positions[0]

        perimeter += abs(x - next_x)
        perimeter += abs(y - next_y)

    return perimeter

def pm(positions):
    max_y = max(positions, key=lambda x: x[1])[1]
    max_x = max(positions, key=lambda x: x[0])[0]
    for y in range(max_y + 1):
        res = ""
        for x in range(max_x + 1):
            if (x, y) in positions:
                res += '#'
            else:
                res += '.'
        
# positions = get_positions("input_test.txt")
positions = get_positions("input.txt")
# positions = [
    # (1, 6),
    # (3, 1),
    # (7, 2),
    # (4, 4),
    # (8, 5)
# ]
# pm(positions)
area = get_area(positions) # shoelace
perimeter = get_perimeter(positions) # chaos
#picks theorem

result = area + (perimeter / 2) + 1
print(result)

