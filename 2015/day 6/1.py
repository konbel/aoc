lights = [[0 for i in range(1000)] for j in range(1000)]

for l in open("input.txt").readlines():
    action = 2
    if l.startswith("turn on"):
        l = l.replace("turn on", "")
        action = 0
    
    elif l.startswith("turn off"):
        l = l.replace("turn off", "")
        action = 1

    else:
        l = l.replace("toggle", "")

    start, end = l.split(" through ")
    start = list(map(int, start.split(",")))
    end = list(map(int, end.strip().split(",")))

    if action == 0:
        for i in range(start[0], end[0] + 1):
            for j in range(start[1], end[1] + 1):
                lights[i][j] = 1

    elif action == 1:
        for i in range(start[0], end[0] + 1):
            for j in range(start[1], end[1] + 1):
                lights[i][j] = 0
        
    else:
        for i in range(start[0], end[0] + 1):
            for j in range(start[1], end[1] + 1):
                lights[i][j] = 1 if lights[i][j] == 0 else 0

print(sum(i.count(1) for i in lights))