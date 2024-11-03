unique = set()
position = (0, 0)

for i in open("input.txt").read():
    match i:
        case "^":
            position = (position[0], position[1] + 1)
        case "v":
            position = (position[0], position[1] - 1)
        case ">":
            position = (position[0] + 1, position[1])
        case "<":
            position = (position[0] - 1, position[1])

    unique.add(position)

print(len(unique) + 1 if len(unique) % 2 != 0 else len(unique))