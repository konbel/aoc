import re

race_length = 2503

reindeer = {}
for line in open("input.txt").readlines():
    n = line.split()[0]
    v, t, r = map(int, re.findall(r"\d+", line))
    reindeer[n] = (v, t, r)

stats = {n: [0, 0, t] for n, (_, t, _) in reindeer.items()}
for i in range(race_length):
    for n, (_, d, s) in stats.items():
        if s > 0:
            stats[n][1] += reindeer[n][0]
            stats[n][2] -= 1

            if stats[n][2] == 0:
                stats[n][2] = -reindeer[n][2]
        else:
            stats[n][2] += 1

            if stats[n][2] == 0:
                stats[n][2] = reindeer[n][1]

    max_distance = max(stats.values(), key=lambda x: x[1])[1]
    winners = [n for n, (_, d, _) in stats.items() if d == max_distance]
    for n in winners:
        stats[n][0] += 1

print(max(stats.values(), key=lambda x: x[0])[0])