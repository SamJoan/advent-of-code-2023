import sys
from pprint import pprint

def convert(l):
    return tuple(convert(x) for x in l) if type(l) is list else l

def parse_patterns(filename):
    fh = open(filename)
    patterns = []
    pattern = []
    for line in fh:
        line = line.strip()
        if line == '':
            patterns.append(pattern)
            pattern = []
            continue

        pattern.append(list(line))

    patterns.append(pattern)
    patterns = convert(patterns)

    return patterns

def identify_mirror_h(pattern, smudge_pos=None):
    for pos in range(len(pattern[0])-1):
        print(pos)
        all_equal = True
        nb_diff = 0
        last_eq_diff_pos = None
        for p_pos, line in enumerate(pattern):
            next_pos = pos+1
            if line[pos] != line[next_pos]:
                all_equal = False
                nb_diff += 1
                last_eq_diff_pos = [pos, (p_pos, pos), (p_pos, next_pos)]

        if all_equal or nb_diff == 1:
            orig_pos = pos
            print("eql %s" % pos)
            nb_different = 0
            all_equal = True
            last_diff_pos = None
            while True:
                pos = pos-1
                # print(pos, next_pos, nb_different)
                next_pos = next_pos+1
                try:
                    if pos == -1:
                        raise IndexError

                    for p_pos, line in enumerate(pattern):
                        if line[pos] != line[next_pos]:
                            all_equal = False
                            nb_different += 1
                            # print '"',''.join(line), '"', pos, next_pos, line[pos], line[next_pos]
                            last_diff_pos = [orig_pos, (p_pos, pos), (p_pos, next_pos)]
                except IndexError:
                    # print(nb_diff, smudge_pos, orig_pos, last_eq_diff_pos)
                    if all_equal:
                        if nb_diff == 0 and smudge_pos != None and smudge_pos == orig_pos:
                            return orig_pos + 1, None
                        if nb_diff == 1 and smudge_pos == None:
                            return -1, last_eq_diff_pos
                        else: 
                            break
                    else:
                        break
                
            print("orig_pos, nb_different, last_diff_pos", orig_pos, nb_different, last_diff_pos)
            if not all_equal:
                if nb_different == 1 and smudge_pos == None:
                    pos, option1, option2 = last_diff_pos
                    print "found smudge!! :((((( %s, %s, %s" % (pos, option1, option2)
                    return -1, last_diff_pos

    return 0, None

def flip_pattern(pattern):
    flipped = {}
    for x in range(len(pattern[0])):
        for y in range(len(pattern)):

            dst_x = x
            if not dst_x in flipped:
                flipped[dst_x] = {}
            if not y in flipped[dst_x]:
                flipped[dst_x][y] = {}

            c = pattern[y][x]

            flipped[dst_x][y] = c

    flipped_list = []
    for line in flipped:
        flipped_line = flipped[line]
        row = []
        # for x in reversed(sorted(flipped_line)):
        for x in flipped_line:
            row.append(flipped_line[x])

        flipped_list.append(row)

    flipped_tuple = convert(flipped_list)
    
    return flipped_tuple

def identify_mirror_v(pattern, smudge=None):
    flipped_tuple = flip_pattern(pattern)

    return identify_mirror_h(flipped_tuple, smudge)

def p(pattern):
    for line in pattern:
        print ''.join(line)

def flip_tuple_coord(pattern, smudge_coords):
    p_pos, pos = smudge_coords
    new = []
    for y, row in enumerate(pattern):
        new_row = []
        for x, c in enumerate(row):
            if y == p_pos and x == pos:
                new_c = '#' if c == '.' else '.'
                new_row.append(new_c)
            else:
                new_row.append(c)

        new.append(new_row)

    new_tuple = convert(new)
    return new
            

# patterns = parse_patterns('input_test.txt')
# patterns = parse_patterns('input_test_mine.txt')
patterns = parse_patterns('input.txt')

# ret = flip_pattern([[
                         # '0', '1', '2'],
                        # ['3', '4', '5']
                        # ])
# ret = flip_pattern([[
                         # '0', '1'],
                        # ['2', '3'],
                        # ['4', '5'],
                        # ])
# print(ret)
# sys.exit()

score = 0
for nb_pattern, pattern in enumerate(patterns):
    found_smudge = False
    # print("PATT")
    # p(pattern)
    # print("FLIPADAPATT")
    # p(flip_pattern(pattern))
    h_score, smudge_coords_list = identify_mirror_h(pattern)
    # print("h_score", h_score)
    if h_score == -1:
        smudge_pos = smudge_coords_list.pop(0)
        for smudge_coords in smudge_coords_list:
            found_smudge = True
            pattern = flip_tuple_coord(pattern, smudge_coords)
            h_score, smudge_coords = identify_mirror_h(pattern, smudge_pos=smudge_pos)
            # print("hscore %s %s" % (h_score, smudge_coords))
            v_score = 0
            if h_score > 0:
                break
    else:
        v_score, smudge_coords_list = identify_mirror_v(pattern)
        # print("v_score", v_score)
        if v_score == -1:
            smudge_pos = smudge_coords_list.pop(0)
            for smudge_coords in smudge_coords_list:
                found_smudge = True
                p_pos, pos = smudge_coords
                pattern = flip_tuple_coord(pattern, (pos, p_pos))
                p(flip_pattern(pattern))
                h_score = 0
                v_score, smudge_coords = identify_mirror_v(pattern, smudge=smudge_pos)
                # print("v_score", v_score)
                if v_score > 0:
                    break

    if not found_smudge:
        print "didn't find smudge. pattern nb %s" % (nb_pattern)
        sys.exit()

    if h_score == -1 or v_score == -1:
        print("Something is seriously wrong... pattern nb %s/%s" % (nb_pattern, len(patterns)))
        sys.exit()

    score += h_score
    score += v_score*100
    
    print('rowscore', h_score, v_score)


print(score)
