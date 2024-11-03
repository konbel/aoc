def generate_combinations(ingredients, target, current_combination=None, current_sum=0):
    if current_combination is None:
        current_combination = {ingredient: 0 for ingredient in ingredients}

    if current_sum == target:
        yield current_combination
    elif current_sum < target and ingredients:
        for i in range(target - current_sum + 1):
            yield from generate_combinations(
                ingredients[1:],
                target,
                {**current_combination, ingredients[0]: i},
                current_sum + i
            )

ingredients = []
properties = {}
for line in open("input.txt").readlines():
    ingredient = line.split(":")[0]
    property = [p.split(" ") for p in line.split(":")[1].strip().split(", ")]

    ingredients.append(ingredient)
    properties[ingredient] = property

max_score = 0
for combination in generate_combinations(ingredients, 100):
    scores = {"capacity": 0, "durability": 0, "flavor": 0, "texture": 0, "calories": 0}

    for i in combination:
        for p in properties[i]:
            scores[p[0]] += combination[i] * int(p[1])

    if scores["calories"] != 500:
        continue

    score = 1
    calories = 0
    for s in scores:
        if s != "calories":
            score *= max(0, scores[s])

    if score > max_score:
        max_score = score

print(max_score)