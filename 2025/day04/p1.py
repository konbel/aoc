import sys


def count_adjacent(grid: list[list[str]], x: int, y: int) -> int:
    count = 0

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
                count += 1
            
    return count


res = 0

input_file = sys.argv[1]
with open(input_file) as file:
    grid = []
    lines = file.readlines()
    for line in lines:
        grid.append([i for i in line.rstrip()])

    for y in range(len(grid)):
        for x in range(len(grid[y])):
            if grid[y][x] == ".":
                continue
            count = count_adjacent(grid, x, y)
            if count < 4:
                res += 1

print(res)
