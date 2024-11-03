total = 0

for l in open("input.txt").read().splitlines():
    l, w, h = map(int, l.split("x"))

    total += min(2 * l + 2 * w, 2 * w + 2 * h, 2 * l + 2 * h)
    total += l * w * h

print(total)