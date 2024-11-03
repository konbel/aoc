import hashlib

input = open('input.txt').read()
i = 0
while True:
    if hashlib.md5((input + str(i)).encode()).hexdigest().startswith('00000'):
        print(i)
        break

    i += 1