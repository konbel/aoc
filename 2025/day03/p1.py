import sys

res = 0

class Battery:
    val: int = -1
    idx: int = -1

def find_two_biggest(bank: str) -> tuple[Battery, Battery]:
    biggest = Battery()
    second_biggest = Battery()

    for i, b in enumerate(bank):
        v = int(b)
        if v > biggest.val:
            second_biggest.val = biggest.val
            second_biggest.idx = biggest.idx

            biggest.val = v
            biggest.idx = i
            continue

        if v > second_biggest.val:
            second_biggest.val = v
            second_biggest.idx = i
    
    return (biggest, second_biggest)


def get_bank_value(first: int, second: int) -> int:
    return int(f"{first}{second}")


def evaluate_bank(bank: str) -> int:
    biggest, second_biggest = find_two_biggest(bank)

    if biggest.idx < second_biggest.idx:
        return get_bank_value(biggest.val, second_biggest.val)
    else:
        if biggest.idx == len(bank) - 1:
            return get_bank_value(second_biggest.val, biggest.val)

        sub_bank = bank[biggest.idx:]
        biggest, second_biggest = find_two_biggest(sub_bank)
        return get_bank_value(biggest.val, second_biggest.val)


input_file = sys.argv[1]
with open(input_file) as file:
    for line in file.readlines():
        res += evaluate_bank(line.rstrip())
print(res)
