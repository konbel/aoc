import sys
from collections import deque, defaultdict


input_file = sys.argv[1]
with open(input_file) as file:
    graph = {}

    for line in file.readlines():
        node, right = line.strip().split(": ")
        connections = set(right.split(" "))
        graph[node] = connections

    topography = defaultdict(int)
    queue = deque(["you"])
    while queue:
        current = queue.popleft()
        topography[current] += 1
        if current in graph:
            for connection in graph[current]:
                queue.append(connection)

    print(topography["out"])
