N = 33100000
houses = {n: 0 for n in range(1, N // 10)}
for i in range(1, N // 10):
    for j in range(i, i + 50 * i, i):
        if j >= N // 10:
            break
        houses[j] += i * 11

for house, presents in houses.items():
    if presents >= N:
        print(house)
        break