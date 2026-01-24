import sys


def parse_line(line: str) -> tuple[int, int]:
    x, y = line.rstrip().split(",")
    return (int(x), int(y))


input_file = sys.argv[1]
with open(input_file) as file:
    tiles = [parse_line(line) for line in file.readlines()]

    largest = 0
    for i in range(len(tiles)):
        a = tiles[i]
        for j in range(len(tiles) - i - 1):
            b = tiles[i + j]

            w = abs(b[0] - a[0]) + 1
            h = abs(b[1] - a[1]) + 1
            area = w * h
            if area > largest:
                largest = area

    print(largest)
