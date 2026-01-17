import sys
from collections import defaultdict


input_file = sys.argv[1]
with open(input_file) as file:
    res = 0

    grid = [[c == "^" for c in line.strip()] for line in file.readlines()]

    beams = {len(grid[0]) // 2: 1}
    for i in range(len(grid)):
        new_beams = defaultdict(int)
        line = grid[i]
        for b, w in beams.items():
            if line[b]:
                res += 1
                new_beams[b - 1] += w
                new_beams[b + 1] += w
            else:
                new_beams[b] += w
        beams = new_beams
    
    print(sum(beams.values()))
