from collections import defaultdict

pairs = []
cities = set()

for line in open("input.txt").readlines():
    start, end = line.split(" to ")
    end, distance = end.split(" = ")
    pairs.append((start, end, int(distance)))
    cities.add(start)
    cities.add(end)

graph = defaultdict(dict)
for start, end, distance in pairs:
    graph[start][end] = distance
    graph[end][start] = distance

def dfs(current, cities, visited, distance):
    visited.add(current)

    if len(visited) == len(cities):
        distances.append(distance)

    for neighbor in graph[current]:
        if neighbor not in visited:
            dfs(neighbor, cities, visited.copy(), distance + graph[current][neighbor])

distances = []

for start in cities:
    dfs(start, cities, set(), 0)

print(max(distances))