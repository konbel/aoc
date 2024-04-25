total_code_chars = 0
total_memory_chars = 0

for line in open("input.txt").readlines():
    line = line.strip()

    total_code_chars += len(line)

    line = line[1:-1]

    i = 0
    while i < len(line):
        if line[i] == '\\':
            if line[i + 1] == 'x':
                i += 3
            else:
                i += 1
        
        total_memory_chars += 1
        i += 1

print(total_code_chars - total_memory_chars)