import sys

res = 0

class Battery:
    val: int = -1
    idx: int = -1


def find_biggest(bank: str) -> Battery:
    biggest = Battery()
    for i, b in enumerate(bank):
        v = int(b)
        if v > biggest.val:
            biggest.val = v
            biggest.idx = i
    return biggest


def get_bank_value(first: int, second: int) -> int:
    return int(f"{first}{second}")


def evaluate_bank(bank: str, current: str = "", digits_after: int = 11) -> str:
    bank_size = len(bank)
    sub_bank = bank[:bank_size - digits_after]

    biggest = find_biggest(sub_bank)
    current += str(biggest.val)

    if digits_after == 0:
        return current

    return evaluate_bank(bank[biggest.idx + 1:], current, digits_after - 1)


input_file = sys.argv[1]
with open(input_file) as file:
    for line in file.readlines():
        res += int(evaluate_bank(line.rstrip()))
print(res)
