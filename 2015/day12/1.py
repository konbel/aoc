import re

line = open("input.txt").read()

sum = 0
for n in re.findall(r'-?\d+', line):
        sum += int(n)
print(sum)