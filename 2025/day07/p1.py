import sys


input_file = sys.argv[1]
with open(input_file) as file:
    res = 0

    grid = [[c == "^" for c in line.strip()] for line in file.readlines()]

    beams = set([len(grid[0]) // 2])
    for i in range(len(grid)):
        new_beams = set()
        line = grid[i]
        for b in beams:
            if line[b]:
                res += 1
                if b - 1 >= 0:
                    new_beams.add(b - 1)
                    new_beams.add(b + 1)
                    continue
                new_beams.add(b + 1)
                new_beams.add(b - 1)
            else:
                new_beams.add(b)
        beams = new_beams
    
    print(res)
