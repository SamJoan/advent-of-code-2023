import sys

IN_ROUTINE = 'in'

def parse_routines(lines):
    routines = {}
    for line in lines:
        instructions = []
        name, instructions = line.split('{')
        instructions = instructions[:-1].split(',')

        if name in routines:
            print "already?"
            sys.exit()

        routines[name] = instructions

    return routines

def parse_input(lines):
    objects = []
    for line in lines:
        obj = {}
        attributes = line[1:-1].split(',')
        for a in attributes:
            key, val = a.split('=')
            obj[key] = int(val)
        objects.append(obj)

    return objects

def parse_program(filename):
    fh = open(filename)
    routines = {}
    input = []

    routine_lines = []
    input_lines = []

    parsing_input = False
    for line in fh:
        line = line.strip()

        if line == '':
            parsing_input = True
            continue

        if not parsing_input:
            routine_lines.append(line)
        else:
            input_lines.append(line)

    return parse_routines(routine_lines), parse_input(input_lines)

def eval_condition(condition, part):
    if '<' in condition:
        key, val = condition.split('<')
        val = int(val)
        return part[key] < val
    elif '>' in condition:
        key, val = condition.split('>')
        val = int(val)
        return part[key] > val

    print 'wat?', condition, part
    sys.exit()

def execute(routines, routine_name, part):
    instructions = routines[routine_name]
    for i in instructions:
        if ':' in i:
            condition, call = i.split(':')
            if not eval_condition(condition, part):
                continue
        else:
            call = i

        if call == 'A':
            return True
        elif call == 'R':
            return False
        else:
            return execute(routines, call, part)

    print "hmm", part, routine_name, routines[routine_name]
    sys.exit()

def inspect_parts(routines, parts):
    accepted = []
    rejected = []

    for part in parts:
        if execute(routines, IN_ROUTINE, part):
            accepted.append(part)
        else:
            rejected.append(part)

    return accepted

def split_interval(interval, condition):
    true_interval = dict(interval)
    false_interval = dict(interval)

    key = condition[0]
    operator = condition[1]
    val = int(condition[2:])

    if operator == '<':
        true_interval[key] = (interval[key][0], val -1)
        false_interval[key] = (val, interval[key][1])
    elif operator == '>':
        true_interval[key] = (val+1, interval[key][1])
        false_interval[key] = (interval[key][0], val)

    return true_interval, false_interval

def sum_possible(interval):
    values = []
    for key in interval:
        min, max = interval[key]
        values.append(max+1 - min)

    total = 1
    for nb in values:
        total *= nb

    return total

def calculate_ranges(routines, interval, routine_name):
    instructions = routines[routine_name]
    nb_accepted = 0
    print(routine_name, instructions)
    for i in instructions:
        if ':' in i:
            condition, call = i.split(':')
            true_i, interval = split_interval(interval, condition)

            if call == 'A':
                nb_accepted += sum_possible(true_i)
            elif call != 'R':
                nb_accepted += calculate_ranges(routines, true_i, call)
        else:
            call = i
            if call == 'A':
                nb_accepted += sum_possible(interval)
            elif call != 'R':
                nb_accepted += calculate_ranges(routines, interval, call)

    return nb_accepted

def inspect_ranges(routines):
    i = {'x': (1, 4000), 'm': (1, 4000), 's': (1, 4000), 'a': (1,4000)}
    nb_accepted = calculate_ranges(routines, i, IN_ROUTINE)
    return nb_accepted

# routines, parts = parse_program('test_input.txt')
routines, parts = parse_program('input.txt')
# accepted = inspect_parts(routines, parts)

accepted = inspect_ranges(routines)
print(accepted)
