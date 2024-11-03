total = 0

for l in open("input.txt").readlines():
    if any([b in l for b in ["ab", "cd", "pq", "xy"]]):
        continue

    if any([l[i] == l[i + 1] for i in range(len(l) - 1)]) and sum([l.count(v) for v in ["a", "e", "i", "o", "u"]]) >= 3:
        total += 1

print(total)