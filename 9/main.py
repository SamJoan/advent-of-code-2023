import sys

def parse_reports(filename):
    fh = open(filename)
    reports = []
    for line in fh:
        report = [int(x) for x in line.strip().split(' ')]
        reports.append(report)

    return reports

def reduce_rows(report):
    rows = [report]
    while True:
        row = []
        prev_row = rows[-1]
        for nb, c in enumerate(prev_row):
            if nb == 0:
                continue
            else:
                row.append(c - prev_row[nb - 1])

        rows.append(row)
        all_zero = True
        for r in row:
            if r != 0:
                all_zero = False

        if all_zero:
            break

    return rows

def calculate_next_digit(report):
    rows = reduce_rows(report)
    for nb in range(len(rows)-1, -1, -1):
        if nb != 0:
            row = rows[nb]
            rows[nb - 1].append(rows[nb - 1][-1] + row[-1])
        else:
            break

    return rows[0][-1]

def calculate_prev_digit(report):
    rows = reduce_rows(report)
    for nb in range(len(rows)-1, -1, -1):
        if nb != 0:
            row = rows[nb]
            rows[nb - 1].insert(0, rows[nb - 1][0] + (-1 * row[0]))
        else:
            break

    return rows[0][0]

# reports = parse_reports('input_test.txt')
reports = parse_reports('input.txt')

total = 0
for report in reports:
    # digit = calculate_next_digit(report)
    digit = calculate_prev_digit(report)
    total += digit

print(total)
