import sys
import re
import z3


input_file = sys.argv[1]
with open(input_file) as f:
    total_presses = 0

    for line in f:
        pattern = re.compile(r"\[(.*?)\]\s(.*?)\s\{(.*?)\}")
        _, buttons, joltages = pattern.match(line).groups()

        buttons = [list(map(int, button.removeprefix("(").removesuffix(")").split(","))) for button in buttons.split(" ")]
        joltages = list(map(int, joltages.split(",")))

        o = z3.Optimize()
        vars = z3.Ints(f"n{i}" for i in range(len(buttons)))
        for var in vars: o.add(var >= 0)
        for i, joltage in enumerate(joltages):
            equation = 0
            for b, button in enumerate(buttons):
                if i in button:
                    equation += vars[b]
            o.add(equation == joltage)
        o.minimize(sum(vars))
        o.check()
        total_presses += o.model().eval(sum(vars)).as_long()

    print(total_presses)
