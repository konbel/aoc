input = "1113122113"
n = 40

def look_and_say(input):
    counts = []

    i = 0
    while i < len(input):
        c = input[i]
        j = i

        while j < len(input) and input[j] == c:
            j += 1

        count = j - i
        i += j - i

        counts.append((c, count))

    return "".join([str(count) + c for c, count in counts])

for i in range(n):
    input = look_and_say(input)

print(len(input))