total = 0

for l in open("input.txt").readlines():
    if any([l[i] == l[i + 2] for i in range(len(l) - 2)]):
        for i in range(len(l) - 1):
            if l.count(l[i] + l[i + 1]) > 1:
                break
        else:
            continue

        total += 1

print(total)