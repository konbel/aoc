import itertools
import math

lines = ()
with open('input.txt') as file:
    lines = file.read().splitlines()
    lines = [int(l) for l in lines]

total_weight = sum(lines)
weight = total_weight // 4

valid_combinations = []
for l in range(len(lines) + 1):
    found_combination = False
    for subset in itertools.combinations(lines, l):
        if sum(subset) == weight:
            found_combination = True
            valid_combinations.append(subset)
    if found_combination:
        break

valid_combinations.sort(key=lambda c: len(c))

shortest_combinations = [c for c in valid_combinations if len(c) == len(valid_combinations[0])]

quantum_entanglement = [math.prod(q) for q in shortest_combinations]
quantum_entanglement.sort()

print(quantum_entanglement[0])
