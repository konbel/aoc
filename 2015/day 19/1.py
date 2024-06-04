from collections import defaultdict

lines = open("input.txt").read().splitlines()
medicine = lines.pop()

replacements = defaultdict(list)
for line in lines:
    if line:
        a, b = line.split(" => ")
        replacements[a].append(b)

total = set()
for n in range(len(medicine)):
    if medicine[n] in replacements:
        for r in replacements[medicine[n]]:
            total.add(medicine[:n] + r + medicine[n+1:])

for n in range(len(medicine)):
    if medicine[n:n+2] in replacements:
        for r in replacements[medicine[n:n+2]]:
            total.add(medicine[:n] + r + medicine[n+2:])

print(len(total))