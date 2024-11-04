import copy

boss_health = 51
boss_damage = 9
player_health = 50
player_mana = 500

all_possible_results = set()

stack = []

def play_round(bh, ph, pm, ce, tm, nx):
    if len(all_possible_results) > 0 and tm > min(all_possible_results):
        return

    # apply effect
    if 'shield' in ce:
        ce['shield'] -= 1

        if ce['shield'] == 0:
            del(ce['shield'])

    if 'poison' in ce:
        bh -= 3
        ce['poison'] -= 1

        if ce['poison'] == 0:
            del(ce['poison'])

    if 'recharge' in ce:
        pm += 101
        ce['recharge'] -= 1

        if ce['recharge'] == 0:
            del(ce['recharge'])

    # check if can apply spell
    if nx == 'missile' and pm < 53:
        return

    if nx == 'drain' and pm < 73:
        return

    if nx == 'shield' and pm < 113:
        return
    
    if nx == 'poison' and pm < 173:
        return

    if nx == 'recharge' and pm < 229:
        return

    # apply spell
    match nx:
        case 'missile':
            pm -= 53
            tm += 53
            bh -= 4

        case 'drain':
            pm -= 73
            tm += 73
            bh -= 2
            ph += 2

        case 'shield':
            pm -= 113
            tm += 113
            ce['shield'] = 6

        case 'poison':
            pm -= 173
            tm += 173
            ce['poison'] = 6

        case 'recharge':
            pm -= 229
            tm += 229
            ce['recharge'] = 5

    # apply effect
    if 'shield' in ce:
        ce['shield'] -= 1

        if ce['shield'] == 0:
            del(ce['shield'])

    if 'poison' in ce:
        bh -= 3
        ce['poison'] -= 1

        if ce['poison'] == 0:
            del(ce['poison'])

    if 'recharge' in ce:
        pm += 101
        ce['recharge'] -= 1

        if ce['recharge'] == 0:
            del(ce['recharge'])

    # check if boss died
    if int(bh) <= 0:
        all_possible_results.add(tm)
        return

    # boss attack
    if 'shield' in ce:
        ph -= boss_damage - 7
    else:
        ph -= boss_damage

    # check if player died
    if int(ph) <= 0:
        return

    # continue game if both alive
    for n in ['missile', 'drain', 'shield', 'poison', 'recharge']:
        if n in ce and ce[n] > 1:
            continue
        new_values = (bh, ph, pm, ce, tm, n)
        stack.append(copy.deepcopy(new_values))

for n in ['missile', 'drain', 'shield', 'poison', 'recharge']:
    values = (boss_health, player_health, player_mana, {}, 0, n)
    stack.append(copy.deepcopy(values))

while len(stack) > 0:
    bh, ph, pm, ce, tm, n = stack.pop()
    play_round(bh, ph, pm, ce, tm, n)

print(all_possible_results)
print(min(all_possible_results))
