import sys
from functools import reduce


def get_problems(number_lines: list[str]) -> list[list[int]]:
    problems = []
    problem = []

    depth = len(number_lines)
    count = len(number_lines[0])
    for i in range(count):
        c = ""
        for d in range(depth):
            c += number_lines[d][i]
        if not c.isspace():
            problem.append(int(c))
            c = ""
        else:
            problems.append(problem)
            problem = []

    if len(problem) > 0:
        problems.append(problem)

    return problems


def get_operators(line: str) -> list[str]:
    operators = []
    for c in line:
        if c == "*" or c == "+":
            operators.append(c)
    return operators


input_file = sys.argv[1]
with open(input_file) as file:
    lines = [line.strip("\n") for line in file.readlines()]
    number_lines = lines[:-1]
    operator_line = lines[-1]

    problems = get_problems(number_lines)
    operators = get_operators(operator_line)

    res = 0
    for i, p in enumerate(problems):
        if operators[i] == "+":
            res += sum(p)
        else:
            res += reduce(lambda x, y: x * y, p)
    print(res)
