password = "vzbxxzaa"

def windows(lst, n):
    return [lst[i:i+n] for i in range(len(lst)-n+1)]

def is_valid(password):
    if "i" in password or "o" in password or "l" in password:
        return False
    
    doubles = set()
    for i in windows(password, 2):
        if i[0] == i[1]:
            doubles.add(i)
        
        if len(doubles) >= 2:
            break
    else:
        return False
    
    for i in windows(password, 3):
        if ord(i[0]) + 2 == ord(i[1]) + 1 == ord(i[2]):
            break
    else:
        return False

    return True

def increment(password, i):
    if password[i] == "z":
        password = password[:i] + "a" + password[i + 1:]
        return increment(password, i - 1)
    else:
        return password[:i] + chr(ord(password[i]) + 1) + password[i + 1:]

while not is_valid(password):
    password = increment(password, len(password) - 1)

print(password)