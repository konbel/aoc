total_code_chars = 0
total_encoded_chars = 0

for line in open("input.txt").readlines():
    line = line.strip()

    total_code_chars += len(line)
    total_encoded_chars += len(line) + 2 + line.count('\\') + line.count('"')

print(total_encoded_chars - total_code_chars)