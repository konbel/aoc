registers = { 'a': 1, 'b': 0 }

lines = ()
with open('input.txt') as file:
    lines = file.read().splitlines()

i = 0
while i < len(lines):
    c = lines[i].replace(',', '').split(' ')

    match c[0]:
        case 'hlf':
            registers[c[1]] = registers[c[1]] // 2
            i += 1

        case 'tpl':
            registers[c[1]] = registers[c[1]] * 3
            i += 1

        case 'inc':
            registers[c[1]] += 1
            i += 1

        case 'jmp':
            i += int(c[1])

        case 'jie':
            if registers[c[1]] % 2 == 0:
                i += int(c[2])
            else:
                i += 1

        case 'jio':
            if registers[c[1]] == 1:
                i += int(c[2])
            else:
                i += 1

        case _:
            break

print('a:' + str(registers['a']))
print('b:' + str(registers['b']))
