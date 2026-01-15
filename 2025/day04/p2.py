import sys


def get_adjacent(grid: list[list[str]], x: int, y: int) -> list[(int, int)]:
    adjacent = []

    for dy in range(-1, 2):
        ny = y + dy
        if ny < 0 or ny >= len(grid):
            continue

        for dx in range(-1, 2):
            if dy == 0 and dx == 0:
                continue

            nx = x + dx
            if nx < 0 or nx >= len(grid[ny]):
                continue
            
            c = grid[ny][nx]
            if c == "@":
                adjacent.append((nx, ny))
            
    return adjacent


def get_removables(grid: list[list[str]]) -> list[(int, int)]:
    removeables = []

    for y in range(len(grid)):
        for x in range(len(grid[y])):
            if grid[y][x] == ".":
                continue
            adjacent = get_adjacent(grid, x, y)
            if len(adjacent) < 4:
                removeables.append((x, y))
    
    return removeables


res = 0

input_file = sys.argv[1]
with open(input_file) as file:
    grid = []
    lines = file.readlines()
    for line in lines:
        grid.append([i for i in line.rstrip()])

    while True:
        removeables = get_removables(grid)

        if len(removeables) == 0:
            break

        for x, y in removeables:
            grid[y][x] = "."
            res += 1

print(res)
