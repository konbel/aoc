from typing import Union

registers = {}
wires = []

def parse_line(line):
    items = line.strip().split(" ")
    items.remove("->")
    
    output = items[-1]
    items.remove(output)

    if items[0] == "NOT":
        args = [items[1], "0"]
        operator = items[0]
    
    elif len(items) == 1:
        args = [items[0], "0"]
        operator = "ASSIGN"

    else:
        args = [items[0], items[2]]
        operator = items[1]

    return args, operator, output

def get_value(key) -> Union[bool, int]:
    if key.isdigit():
            return (True, int(key))
    else:
        if key in registers:
            return (True, registers[key])
        
        return (False, 0)

def execute(args, operator, output) -> bool:
    success1, value1 = get_value(args[0])
    success2, value2 = get_value(args[1])

    if not success1 or not success2:
        return False

    match operator:
        case "AND":
            registers[output] = value1 & value2
        
        case "OR":
            registers[output] = value1 | value2
        
        case "LSHIFT":
            registers[output] = value1 << value2

        case "RSHIFT":
            registers[output] = value1 >> value2

        case "NOT":
            registers[output] = ~value1

        case "ASSIGN":
            registers[output] = value1

    registers[output] = registers[output] & 0xffff

    return True

for line in open("input2.txt").readlines():
    wires.append(parse_line(line))

index = 0
while len(wires) > 0:
    if index >= len(wires):
        index = 0

    wire = wires[index]
    if execute(wire[0], wire[1], wire[2]):
        if wire[2] == "a":
            break

        wires.remove(wire)
    else:
        index += 1

print(registers["a"])