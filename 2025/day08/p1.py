import sys


class Vec3:
    def __init__(self, x: int = 0, y: int = 0, z: int = 0):
        self.x = x
        self.y = y
        self.z = z
    

    def __eq__(self, other):
        return isinstance(other, Vec3) and self.x == other.x and self.y == other.y and self.z == other.z
    

    def __hash__(self):
        return hash((self.x, self.y, self.z))


    def distance_squared(self, other) -> float:
        dx = other.x - self.x
        dy = other.y - self.y
        dz = other.z - self.z
        return dx * dx + dy * dy + dz * dz


class Pair:
    def __init__(self, a: Vec3, b: Vec3, dist: float):
        self.a = a
        self.b = b
        self.dist = dist


def calculate_distances(positions: list[Vec3]) -> list[Pair]:
    pairs = []
    for i in range(len(positions)):
        p1 = positions[i]
        for p2 in positions[i + 1:]:
            pairs.append(Pair(p1, p2, p1.distance_squared(p2)))
    pairs.sort(key=lambda p: p.dist)
    return pairs


def calculate_circuits(positions: list[Vec3], pairs: list[Pair]) -> list[set[Pair]]:
    circuits = [set([p]) for p in positions]
    for p in pairs:
        for c in circuits:
            if p.a in c:
                c1 = c
            if p.b in c:
                c2 = c
        
        if c1 is c2:
            continue

        for v in c2:
            c1.add(v)
        circuits.remove(c2)

    circuits.sort(key=len, reverse=True)
    return circuits


input_file = sys.argv[1]
with open(input_file) as file:
    positions = []
    for line in file.readlines():
        x, y, z = line.rstrip().split(",")
        positions.append(Vec3(int(x), int(y), int(z)))

    pairs = calculate_distances(positions)
    circuits = calculate_circuits(positions, pairs[:1000])

    res = 1
    count = min(3, len(circuits))
    for i in range(count):
        res *= len(circuits[i])
    print(res)
