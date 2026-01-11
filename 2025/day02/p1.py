import sys

sum = 0

input_file = sys.argv[1]
with open(input_file) as file:
    ranges = file.readline().split(",")
    for r in ranges:
        lower, upper = [int(i) for i in r.split("-")]
        for i in range(lower, upper + 1):
            candidate = str(i)
            l = len(candidate)
            if l % 2 != 0:
                continue
            l2 = l // 2
            p1 = candidate[:l2]
            p2 = candidate[l2:]
            if p1 == p2:
                sum += int(candidate)

print(sum)