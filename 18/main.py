import sys
from pprint import pprint

def get_positions(filename):
    fh = open(filename)
    orig_pos = (1, 1)
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
        print(shoelace)
        area += shoelace

    return area

positions = get_positions("input_test.txt")
print(positions)
# positions = [
    # (1, 6),
    # (3, 1),
    # (7, 2),
    # (4, 4),
    # (8, 5)
# ]
area = get_area(positions)
print(area)

