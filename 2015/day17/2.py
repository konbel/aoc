import itertools

def find_min_containers(containers, litres):
    for i in range(1, len(containers)):
        for c in itertools.combinations(containers, i):
            if sum(c) == litres:
                return i

litres = 150
containers = [int(line) for line in open("input.txt")]

min_containers = find_min_containers(containers, litres)
combinations = 0    
for c in itertools.combinations(containers, min_containers):
    if sum(c) == litres:
        combinations += 1

print(combinations)