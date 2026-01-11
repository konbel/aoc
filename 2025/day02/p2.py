import sys

sum = 0

def evaluate(candidate: str) -> bool:
    l = len(candidate) // 2 + 1
    for i in range(l):
        c = candidate[:i]
        res = candidate.replace(c, "")
        if len(res) == 0:
            return True
    return False


input_file = sys.argv[1]
with open(input_file) as file:
    ranges = file.readline().split(",")
    for r in ranges:
        lower, upper = [int(i) for i in r.split("-")]
        for i in range(lower, upper + 1):
            candidate = str(i)
            if evaluate(candidate):
                sum += i

print(sum)