import sys, re


input_file = sys.argv[1]
with open(input_file) as file:
    lines = file.read().split("\n\n")[-1].splitlines()

    total = 0

    for line in lines:
        x, y, *counts = list(map(int, re.findall(r"\d+", line)))
        if (x // 3) * (y // 3) >= sum(counts):
            total += 1

    print(total)
