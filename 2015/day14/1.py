import re

race_length = 2503
max_distance = 0
for line in open("input.txt").readlines():
    n = line.split()[0]
    v, t, r = map(int, re.findall(r"\d+", line))

    c = race_length // (t + r)
    distance = c * v * t

    rem = race_length % (t + r)
    if rem > t:
        distance += v * t
    else:
        distance += v * rem

    if distance > max_distance:
        max_distance = distance

print(max_distance)