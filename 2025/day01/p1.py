import sys

rotation = 50
res = 0

input_file = sys.argv[1]
with open(input_file) as file:
    for line in file.readlines():
        dir = 1 if line[0] == 'R' else -1
        count = int(line[1:])
        rotation = (rotation + dir * count) % 100

        if rotation == 0:
            res += 1

print(res)
