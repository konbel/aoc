lines = open("input.txt").read().splitlines()
boss_hp = 104
boss_damage = 8
boss_armor = 1
player_hp = 100

weapons = [
    [8, 4, 0],
    [10, 5, 0],
    [25, 6, 0],
    [40, 7, 0],
    [74, 8, 0]
]

armors = [
    [0, 0, 0],
    [13, 0, 1],
    [31, 0, 2],
    [53, 0, 3],
    [75, 0, 4],
    [102, 0, 5]
]

rings = [
    [0, 0, 0],
    [25, 1, 0],
    [50, 2, 0],
    [100, 3, 0],
    [20, 0, 1],
    [40, 0, 2],
    [80, 0, 3]
]

min_cost = 10000
for weapon in weapons:
    for armor in armors:
        for ring1 in rings:
            for ring2 in rings:
                if ring1 == ring2 and ring1 != 0:
                    continue
                c = weapon[0] + armor[0] + ring1[0] + ring2[0]
                d = weapon[1] + armor[1] + ring1[1] + ring2[1]
                a = weapon[2] + armor[2] + ring1[2] + ring2[2]
                if -((boss_hp - 1) // -max(1, d - boss_armor)) <= -((player_hp - 1) // -max(1, boss_damage - a)):
                    if c < min_cost:
                        min_cost = c

print(min_cost)