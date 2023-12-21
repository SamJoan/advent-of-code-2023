import sys

def init_map():
    size = 2000
    map = {}
    for y in range(size):
        map[y] = {}
        for x in range(size):
            map[y][x] = '.'

    return map

def dig(map, pos):
    x, y = pos
    map[y][x] = '#'

def pm(map):
    max_x = 0
    strs = []
    for y in range(len(map)):
        str = ""

        contains_no_nb = True
        for x in range(len(map[y])):
            c = map[y][x]
            str += c
            if c == '#':
                contains_no_nb = False
                max_x = x if x > max_x else max_x

        if contains_no_nb:
            break

        strs.append(str)

    for str in strs:
        print(str[0:max_x+1])

def dig_map(filename):
    fh = open(filename)
    map = init_map()
    pos = (1000, 1000)
    dig(map, pos)
    for line in fh:
        line = line.strip()
        dir, distance, colour = line.split(' ')
        x, y = pos
        for i in range(1, int(distance) + 1):
            if dir in ['R', 'L']:
                s = -1 if dir == 'L' else 1
                x = x + 1 * s
            else:
                s = -1 if dir == 'U' else 1
                y = y + 1 * s

            if x < 0 or y < 0:
                print "bad ", x, y
                sys.exit()

            pos = (x, y)
            print(pos)
            pm(map)
            dig(map, pos)

    for y in range(len(map)):
        nb_crossed = 0
        prev_char_dug = False

        for x in range(len(map[0])):
            c = map[y][x]
            is_inside = nb_crossed % 2 == 1
            # if x < 12 and y < 12:
                # print(x,y, is_inside, nb_crossed)
                # pm(map)
                # raw_input()

            if c == '#' and not prev_char_dug:
                if map[y + 1][x] == '#':
                    nb_crossed += 1
                    prev_char_dug = True
            else:
                prev_char_dug = False
                if is_inside:
                    dig(map, (x, y))

    return map


# map = dig_map("input_test.txt")
map = dig_map("input.txt")

count = 0
for y in range(len(map)):
    for x in range(len(map[0])):
        c = map[y][x]
        if c == '#':
            count += 1

print(count)

