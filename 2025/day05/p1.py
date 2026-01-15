import sys

type Range = tuple[int, int]


def check_range(range: Range, id: int) -> bool:
    return range[0] <= id and id <= range[1]


input_file = sys.argv[1]
with open(input_file) as file:
    res = 0

    lines = [line.rstrip() for line in file.readlines()]
    empty_index = lines.index("")
    ranges = lines[:empty_index]
    ids = lines[empty_index + 1:]

    fresh = []
    for r in ranges:
        start, end = r.split("-")
        f = (int(start), int(end))
        fresh.append(f)
    
    for i in ids:
        v = int(i)
        for r in fresh:
            if check_range(r, v):
                res += 1
                break

print(res)
