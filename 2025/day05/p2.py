import sys

type Range = tuple[int, int]


def check_range(range: Range, id: int) -> bool:
    return range[0] <= id and id <= range[1]


def check_overlap(a: Range, b: Range) -> bool:
    return check_range(a, b[0]) or check_range(a, b[1]) or check_range(b, a[0]) or check_range(b, a[1])


def join_ranges(a: Range, b: Range) -> Range:
    lower = min(a[0], b[0])
    upper = max(a[1], b[1])
    return (lower, upper)


def combine_all(ranges: list[Range]) -> list[Range]:
    result = []
    while True:
        prev = len(ranges)
        for i, a in enumerate(ranges):
            for j, b in enumerate(ranges):
                if i == j:
                    continue

                if a == b:
                    ranges.remove(b)
                    break

                if check_overlap(a, b):
                    n = join_ranges(a, b)
                    ranges.remove(a)
                    ranges.remove(b)
                    ranges.append(n)
                    break
            else:
                result.append(a)
                ranges.remove(a)
        if prev == len(ranges):
            break
    return result


input_file = sys.argv[1]
with open(input_file) as file:
    res = 0

    lines = [line.rstrip() for line in file.readlines()]
    empty_index = lines.index("")

    ranges = []
    for r in lines[:empty_index]:
        start, end = r.split("-")
        f = (int(start), int(end))
        ranges.append(f)
    
    ranges = combine_all(ranges)

    for r in ranges:
        d = r[1] - r[0]
        res += d + 1


print(res)
