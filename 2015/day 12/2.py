import json
import re

def remove_red(obj):
    if isinstance(obj, dict):
        if "red" in obj.values():
            return None
        else:
            return {k: remove_red(v) for k, v in obj.items()}
    elif isinstance(obj, list):
        return [remove_red(item) for item in obj]
    else:
        return obj

line = json.loads(open("input.txt").read())
line = str(remove_red(line))

sum = 0
for n in re.findall(r'-?\d+', line):
        sum += int(n) 
print(sum)