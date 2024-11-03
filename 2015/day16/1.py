data = {
    "children": 3,
    "cats": 7,
    "samoyeds": 2,
    "pomeranians": 3,
    "akitas": 0,
    "vizslas": 0,
    "goldfish": 5,
    "trees": 3,
    "cars": 2,
    "perfumes": 1
}

max_score = 0
number = 0
for line in open("input.txt"):
    n = int(line.split()[1][:-1])
    
    line = line.strip().replace("Sue " + str(n) + ": ", "")
    values = line.split(", ")
    properties = {}
    for v in values:
        key, value = v.split(": ")
        properties[key] = int(value)

    score = 0
    for key, value in properties.items():
        for k, v in data.items():
            if key == k and value == v:
                score += 1
    
    if score > max_score:
        max_score = score
        number = n

print(number)                