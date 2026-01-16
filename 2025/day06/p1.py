import sys
from functools import reduce


def get_problems(number_lines: list[str]) -> list[list[int]]:
    numbers = []
    for l in number_lines:
        l = l.rstrip()
        nums = []
        current = ""
        for c in l:
            if c.isnumeric():
                current += c
            else:
                if current != "":
                    nums.append(int(current))
                    current = ""
        if current != "":
            nums.append(int(current))
        numbers.append(nums)
    
    count = len(numbers[0])
    depth = len(numbers)
    problems = []
    for c in range(count):
        p = []
        for d in range(depth):
            p.append(numbers[d][c])
        problems.append(p)

    return problems


input_file = sys.argv[1]
with open(input_file) as file:
    lines = file.readlines()
    number_lines = lines[:-1]
    operator_line = lines[-1]

    operators = []
    for c in operator_line:
        if c == "*" or c == "+":
            operators.append(c)
    
    problems = get_problems(number_lines)

    res = 0
    for i, p in enumerate(problems):
        if operators[i] == "+":
            res += sum(p)
        else:
            res += reduce(lambda x, y: x * y, p)
    print(res)
