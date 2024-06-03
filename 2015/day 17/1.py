import itertools

litres = 150
containers = [int(line) for line in open("input.txt")]

combinations = 0
for i in range(1, len(containers)):
    for c in itertools.combinations(containers, i):
        if sum(c) == litres:
            combinations += 1

print(combinations)