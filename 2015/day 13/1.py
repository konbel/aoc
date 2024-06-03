from collections import defaultdict
import itertools

def calculate_happiness(guests, arrangement):
    max_happiness = None
    for arrangement in itertools.permutations(guests):
        total_happiness = sum(happiness[a][b] + happiness[b][a] for a, b in zip(arrangement, arrangement[1:] + arrangement[:1]))
        if max_happiness is None or total_happiness > max_happiness:
            max_happiness = total_happiness

    return max_happiness

happiness = defaultdict(dict)
guests = set()
for line in open("input.txt"):
    line = line.split()

    p1 = line[0]
    p2 = line[-1][:-1]
    
    if line[2] == "gain":
        h = int(line[3])
    else:
        h = -int(line[3])

    happiness[p1][p2] = h
    guests.add(p1)

print(calculate_happiness(guests, happiness))