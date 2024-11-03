from random import shuffle

lines = open("input.txt").read().splitlines()
molecule = lines.pop()
replacements = [(line.split()[0], line.split()[-1]) for line in lines if " => " in line]

target = molecule
mutations = 0
while target != 'e':
    tmp = target
    for a, b in replacements:
        if b not in target:
            continue

        target = target.replace(b, a, 1)
        mutations += 1

    if tmp == target:
        target = molecule
        mutations = 0
        shuffle(replacements)

print(mutations)