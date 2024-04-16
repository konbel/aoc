sum = 0

for (x, i) in enumerate(open("input.txt").read()):
    sum += 1 if i == "(" else -1

    if sum == -1:
        print(x + 1)
        break