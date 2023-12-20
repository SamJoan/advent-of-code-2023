import sys
from collections import OrderedDict
def get_steps(filename):
    fh = open(filename)
    steps = []
    for line in fh:
        line = line.strip()
        line_steps = line.split(',')
        steps += line_steps

    return steps

def hash(step):
    current_value = 0
    for c in step:
        val = ord(c)
        current_value += val
        current_value = current_value * 17
        current_value = current_value % 256

    return current_value

def initialise_boxes():
    boxes = {}
    for nb in range(256):
        boxes[nb] = OrderedDict()

    return boxes

def add_lens(boxes, step):
    label, focal_length = step.split('=')
    nb = hash(label)
    boxes[nb][label] = int(focal_length)

def remove_lens(boxes, step):
    label = step[:-1]
    nb = hash(label)
    try:
        del boxes[nb][label]
    except KeyError:
        pass

def print_boxes(boxes):
    for nb in boxes:
        box = boxes[nb]
        if len(box) > 0:
            print(box)

def get_focusing_power(boxes):
    total_focusing_power = 0
    for nb in boxes:
        box = boxes[nb]
        if len(box) > 0:
            nb_box = nb + 1
            for n, lens in enumerate(box):
                focal_lens = box[lens]
                nb_lens = n + 1
                total_focusing_power += nb_box * nb_lens * focal_lens

    return total_focusing_power
                

# steps = get_steps('input_test.txt')
steps = get_steps('input.txt')

assert hash("HASH") == 52

boxes = initialise_boxes()

for step in steps:
    if '=' in step:
        add_lens(boxes, step)
    elif '-' in step:
        remove_lens(boxes, step)
    else:
        assert False, "What?"

foc_pow = get_focusing_power(boxes)
print(foc_pow)
