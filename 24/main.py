import sys
from pprint import pprint
from itertools import combinations

class Hailstone():
    def __init__(self, pos, velocity):
        self.pos = pos
        self.velocity = velocity

    def get_line(self):
        point_a = (self.pos[0], self.pos[1])
        point_b = (self.pos[0] + self.velocity[0], self.pos[1] + self.velocity[1])
        return point_a, point_b

def parse_hailstones(filename):
    fh = open(filename)
    hs = []
    for line in fh:
        line = line.strip()
        pos, velocity = [x.strip() for x in line.split("@")]
        pos_tpl = tuple([int(x) for x in pos.split(',')])
        velocity_tpl = tuple([int(x) for x in velocity.split(',')])
        h = Hailstone(pos_tpl, velocity_tpl)
        hs.append(h)

    return hs


# https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment
# https://www.symbolab.com/solver/solve-for-x-calculator/solve%20for%20x%2C%20y%20%3D%20p%2B%5Cleft(x%20%5Ccdot%20m%5Cright)?or=input
def determine_collisions(hailstones, min, max):
    in_area = 0
    for ha, hb in combinations(hailstones, 2):
        

        ha_vel = ha.velocity
        pt1, pt2 = ha.get_line()
        x1, y1 = pt1
        x2, y2 = pt2

        pt3, pt4 = hb.get_line()
        x3, y3 = pt3
        x4, y4 = pt4

        nominator_x = ((x1 * y2) - (y1*x2))*(x3 - x4) - (x1 - x2)*((x3*y4)-(y3*x4))
        nominator_y = ((x1*y2)-(y1*x2))*(y3-y4)-((y1-y2)*((x3*y4)-(y3*x4)))
        denominator = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4)

        if denominator == 0:
            continue # parallel

        px = nominator_x / denominator
        py = nominator_y / denominator

        t_ha = (px - ha.pos[0]) / ha.velocity[0]
        t_hb = (px - hb.pos[0]) / hb.velocity[0]

        if t_ha < 0 or t_hb < 0:
            continue

        if px > min and px < max and py > min and py < max:
            in_area += 1

    return in_area



# hailstones = parse_hailstones("input_test.txt")
# collisions = determine_collisions(hailstones, 7, 27)

hailstones = parse_hailstones("input.txt")
collisions = determine_collisions(hailstones, 200000000000000, 400000000000000)

print(collisions)
