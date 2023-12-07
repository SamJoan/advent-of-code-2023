import sys

class Map:
    def __init__(self, name):
        self.rules = []
        self.name = name
        self.lookup = {}


    def translate(self, seed):
        for rule in self.rules:
            translated = rule.translate(seed)
            if translated:
                return translated

        return seed

class TranslationRule:
    def __init__(self, destination, source, range):
        self.dst_start = int(destination)
        self.src_start = int(source)
        self.range = int(range)

    def translate(self, seed):
        min_src = self.src_start
        max_src = self.src_start + self.range - 1
        if seed >= min_src and seed <= max_src:
            position = seed - min_src
            return self.dst_start + position

class SeedGenerator:
    pos = 0

    def __init__(self, start, range):
        self.start = start
        self.range = range

    def __iter__(self):
        self.pos = 0
        return self

    def __next__(self):
        if self.pos < self.range:
            ret = self.start + self.pos
            self.pos += 1
            return ret
        else:
            raise StopIteration

def parse_maps(file):
    fh = open(file)

    i = 0
    seeds = []
    maps = []
    parsing_map = None
    for line in fh:
        line = line.strip()
        parsing_seeds = i == 0
        if parsing_seeds:

            splat = line.split(":")[1]
            split_splat = splat.split(' ')
            split_splat_splot = [x for x in split_splat if x]
            for nb, start in enumerate(split_splat_splot):
                if nb % 2 == 0:
                    range = split_splat_splot[nb + 1]
                    seeds.append(SeedGenerator(int(start), int(range)))

        elif not parsing_map:
            if line == '':
                continue
            name = line.split(' ')[0]
            parsing_map = Map(name)
        else:
            if line == '':
                maps.append(parsing_map)
                parsing_map = None
            else:
                dst_start, src_start, range = line.split(' ')
                rule = TranslationRule(dst_start, src_start, range)
                parsing_map.rules.append(rule)

        i += 1

    maps.append(parsing_map)
    parsing_map = None

    return seeds, maps

def map(maps, seed):
    input = seed
    nb = 0
    for map in maps:
        input = map.translate(input)
        nb += 1

    return input

# seed_generators, maps = parse_maps("input_test.txt")
seed_generators, maps = parse_maps("input.txt")

lowest_number = 9223372036854775807
nb = 0
for seed_generator in seed_generators:
    for seed in seed_generator:

        output = map(maps, seed)
        if output < lowest_number:
            lowest_number = output

        nb += 1

print(lowest_number)
