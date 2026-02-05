import sys
from functools import lru_cache


input_file = sys.argv[1]
with open(input_file) as file:
    graph = {}

    for line in file.readlines():
        node, right = line.strip().split(": ")
        connections = set(right.split(" "))
        graph[node] = connections

    @lru_cache(maxsize=None)
    def count_paths(current: str, dac: bool, fft: bool) -> int:
        if current == "out":
            return 1 if dac and fft else 0

        if current not in graph:
            return 0

        if current == "dac":
            dac = True
        elif current == "fft":
            fft = True

        total = 0
        for connection in graph[current]:
            total += count_paths(connection, dac, fft)
        return total

    print(count_paths("svr", False, False))
