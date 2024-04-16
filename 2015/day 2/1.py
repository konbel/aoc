total = 0

for l in open("input.txt").read().splitlines():
    l, w, h = map(int, l.split("x"))

    total += 2 * l * w + 2 * w * h + 2 * h * l
    total += min(l * w, w * h, h * l)

print(total)