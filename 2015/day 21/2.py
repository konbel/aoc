lines = open("input.txt").read().splitlines()
boss_hp = int(lines[0].split(": ")[1])
boss_damage = int(lines[1].split(": ")[1])
boss_armor = int(lines[2].split(": ")[1])
player_hp = 100

weapons = [
    [0 ,0, 0],
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

max_cost = 0
for weapon in weapons:
    for armor in armors:
        for ring1 in rings:
            for ring2 in rings:
                if ring1 == ring2 and ring1 != 0:
                    continue
                c = weapon[0] + armor[0] + ring1[0] + ring2[0]
                d = weapon[1] + armor[1] + ring1[1] + ring2[1]
                a = weapon[2] + armor[2] + ring1[2] + ring2[2]
                rounds_player_win = -(boss_hp // -max(1, d - boss_armor))
                rounds_player_loose = -(player_hp // -max(1, boss_damage - a))
                if rounds_player_win > rounds_player_loose:

                    if c > max_cost:
                        max_cost = c

print(max_cost)