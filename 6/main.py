
def get_races(filename):
    fh = open(filename)

    times = []
    distances = []
    for nb, line in enumerate(fh):
        line = line.strip()
        _, data = line.split(":")
        data_split = [x for x in data.split(' ') if x != '']
        for t in data_split:
            if nb == 0:
                times.append(t)
            else:
                distances.append(t)

    return times, distances


# times, records = get_races("input_test.txt")
times, records = get_races("input.txt")

results = []
for race_nb in range(len(times)):
    time = int(times[race_nb])
    record = int(records[race_nb])
    win_total = 0
    for speed in range(1, time):
        remaining_ms = time - speed
        total_distance = speed * remaining_ms
        if total_distance > record:
            win_total += 1
        
    results.append(win_total)

total = 1
for nb in results:
    total = total * nb

print(total)
