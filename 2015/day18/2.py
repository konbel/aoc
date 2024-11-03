def next_state(x, y):
    if (x, y) in ((0, 0), (0, len(grid) - 1), (len(grid[0]) - 1, 0), (len(grid[0]) - 1, len(grid) - 1)):
        return "#"

    neighbors_on = 0
    for dy in range(-1, 2):
        for dx in range(-1, 2):
            if dx == 0 and dy == 0:
                continue

            if 0 <= x + dx < len(grid[y]) and 0 <= y + dy < len(grid):
                if grid[y + dy][x + dx] == "#":
                    neighbors_on += 1

    if grid[y][x] == "#":
        if neighbors_on in (2, 3):
            return "#"
        else:
            return "."
    else:
        if neighbors_on == 3:
            return "#"
        else:
            return "."

grid = [l.strip() for l in open("input.txt")]
grid[0] = "#" + grid[0][1:-1] + "#"
grid[-1] = "#" + grid[-1][1:-1] + "#"

for i in range(100):
    new_grid = []
    for y in range(len(grid)):
        new_row = ""
        for x in range(len(grid[y])):
            new_row += next_state(x, y)
        new_grid.append(new_row)
    
    grid = new_grid

print(sum(row.count("#") for row in grid))