import sys
import re
from collections import deque


def config_to_binary(config: str) -> int:
    binary = 0
    for i, char in enumerate(config):
        if char == "#":
            binary += 2**i
    return binary


def parse_button(button: str) -> int:
    binary = 0
    for bit in button.strip("(").strip(")").split(","):
        binary += 2**int(bit)
    return binary


def configure(config: int, buttons: list[int]) -> int:
    queue = deque([(1, button) for button in buttons])
    seen = set(buttons)

    while len(queue) > 0:
        presses, current = queue.popleft()
        if current == config:
            return presses

        for button in buttons:
            new_config = current ^ button

            if new_config not in seen:
                queue.append((presses + 1, new_config))
                seen.add(new_config)

    raise ValueError("No solution found")


input_file = sys.argv[1]
with open(input_file) as f:
    total_presses = 0

    for line in f:
        pattern = re.compile(r"\[(.*?)\]\s(.*?)\s\{(.*?)\}")
        config, buttons, _ = pattern.match(line).groups()

        config = int(config_to_binary(config))
        buttons = list(map(parse_button, buttons.split(" ")))

        total_presses += configure(config, buttons)

    print(total_presses)
