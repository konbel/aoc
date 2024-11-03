unique = set()
total = 0
input = open("input.txt").read()

position_robot = (0, 0)
position_santa = (0, 0)

for (i, j) in zip(input[0::2], input[1::2]):
    match i:
        case "^":
            position_santa = (position_santa[0], position_santa[1] + 1)
        case "v":
            position_santa = (position_santa[0], position_santa[1] - 1)
        case ">":
            position_santa = (position_santa[0] + 1, position_santa[1])
        case "<":
            position_santa = (position_santa[0] - 1, position_santa[1])

    match j:
        case "^":
            position_robot = (position_robot[0], position_robot[1] + 1)
        case "v":
            position_robot = (position_robot[0], position_robot[1] - 1)
        case ">":
            position_robot = (position_robot[0] + 1, position_robot[1])
        case "<":
            position_robot = (position_robot[0] - 1, position_robot[1])

    unique.add(position_santa)
    unique.add(position_robot)

print(len(unique) + 1 if len(unique) % 2 != 0 else len(unique))