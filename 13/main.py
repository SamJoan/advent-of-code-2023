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

def identify_mirror_h(pattern):
    for pos in range(len(pattern[0])-1):
        all_equal = True
        for line in pattern:
            next_pos = pos+1
            if line[pos] != line[next_pos]:
                all_equal = False
                continue

        if all_equal:
            print('lelele', pos)
            orig_pos = pos
            while True:
                pos = pos-1
                next_pos = next_pos+1
                all_equal = True
                try:
                    if pos == -1:
                        raise IndexError

                    for line in pattern:
                        if line[pos] != line[next_pos]:
                            all_equal = False
                except IndexError:
                    return orig_pos + 1
                
                if not all_equal:
                    break


    return 0

def identify_mirror_v(pattern):
    flipped = []
    for row_pos in range(len(pattern[0])):
        row = []

        for pattern_pos in range(len(pattern)):
            row.append(pattern[pattern_pos][row_pos])

        flipped.append(row)
    
    flipped_tuple = convert(flipped)
    print "flipped"
    p(flipped_tuple)
    return identify_mirror_h(flipped_tuple)

def p(pattern):
    for line in pattern:
        print ''.join(line)

# patterns = parse_patterns('input_test.txt')
patterns = parse_patterns('input.txt')

score = 0
for pattern in patterns:
    h_score = identify_mirror_h(pattern)
    v_score = identify_mirror_v(pattern)
    score += h_score
    score += v_score*100
    
    p(pattern)
    print('rowscore', h_score, v_score)
    
    # raw_input()


print(score)
