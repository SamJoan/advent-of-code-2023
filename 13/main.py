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
                            # print(pos, next_pos, nb_different)
                            last_diff_pos = [orig_pos, (p_pos, pos), (p_pos, next_pos)]
                except IndexError:
                    if all_equal:
                        if nb_diff == 0 and smudge_pos == orig_pos:
                            return orig_pos + 1, None
                        if nb_diff == 1:
                            return -1, last_eq_diff_pos
                        else: 
                            break
                    else:
                        break
                
            if not all_equal:
                if nb_different == 1:
                    pos, option1, option2 = last_diff_pos
                    print "found smudge!! :((((( %s, %s, %s" % (pos, option1, option2)
                    p(pattern)
                    return -1, last_diff_pos

    return 0, None

def identify_mirror_v(pattern, smudge=True):
    flipped = []
    for y in range(len(pattern)):
        row = []

        for x in range(len(pattern)):
            row.append(pattern[x][y])

        flipped.append(row)
    
    flipped_tuple = convert(flipped)
    print "FLIPPED"
    p(flipped_tuple)
    assert pattern[0][0] == flipped_tuple[0][0]
    assert pattern[1][0] == flipped_tuple[0][1]
    assert pattern[2][0] == flipped_tuple[0][2]
    assert pattern[3][0] == flipped_tuple[0][3]

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
patterns = parse_patterns('input.txt')

score = 0
for pattern in patterns:
    found_smudge = False
    p(pattern)
    h_score, smudge_coords_list = identify_mirror_h(pattern)
    print("h_score", h_score)
    if h_score == -1:
        smudge_pos = smudge_coords_list.pop(0)
        for smudge_coords in smudge_coords_list:
            found_smudge = True
            pattern = flip_tuple_coord(pattern, smudge_coords)
            h_score, smudge_coords = identify_mirror_h(pattern, smudge_pos=smudge_pos)
            print("hscore %s %s" % (h_score, smudge_coords))
            v_score = 0
            if h_score > 0:
                break
    else:
        v_score, smudge_coords_list = identify_mirror_v(pattern)
        print("v_score '%s'" % (v_score))
        if v_score == -1:
            smudge_pos = smudge_coords_list.pop(0)
            for smudge_coords in smudge_coords_list:
                found_smudge = True
                p_pos, pos = smudge_coords
                pattern = flip_tuple_coord(pattern, (pos, p_pos))
                h_score = 0
                v_score, smudge_coords = identify_mirror_v(pattern, smudge=smudge_pos)
                print("v_score %s %s" % (v_score, smudge_coords))
                if v_score > 0:
                    break

    if not found_smudge:
        print "didn't find smudge."
        sys.exit()

    score += h_score
    score += v_score*100
    
    print('rowscore', h_score, v_score)
    
    # raw_input()


print(score)
