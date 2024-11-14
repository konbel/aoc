#include "../includes.h"
#include "day7.h"

constexpr int FIVE_OF_A_KIND = 7;
constexpr int FOUR_OF_A_KIND = 6;
constexpr int FULL_HOUSE = 5;
constexpr int THREE_OF_A_KIND = 4;
constexpr int TWO_PAIR = 3;
constexpr int ONE_PAIR = 2;
constexpr int HIGH_CARD = 1;

map<char, int> card_values = {
    {'A', 13},
    {'K', 12},
    {'Q', 11},
    {'J', 10},
    {'T', 9},
    {'9', 8},
    {'8', 7},
    {'7', 6},
    {'6', 5},
    {'5', 4},
    {'4', 3},
    {'3', 2},
    {'2', 1}
};

static int value_hand_p1(string hand) {
    std::unordered_map<char, int> card_counts;
    for (char card : hand) card_counts[card]++;

    if (card_counts.size() == 1) return FIVE_OF_A_KIND;
    if (card_counts.size() == 5) return HIGH_CARD;

    int count_2_pair = 0;
    int count_trips = 0;
    int count_quads = 0;
    for (auto i : card_counts) {
        if (i.second == 2) count_2_pair++;
        if (i.second == 3) count_trips++;
        if (i.second == 4) count_quads++;;
    }

    if (count_quads == 1) return FOUR_OF_A_KIND;
    if (count_trips == 1 && count_2_pair == 1) return FULL_HOUSE;
    if (count_trips == 1) return THREE_OF_A_KIND;
    if (count_2_pair == 2) return TWO_PAIR;
    if (count_2_pair == 1) return ONE_PAIR;

    return HIGH_CARD;
}

static int value_hand_p2(string hand) {
    std::unordered_map<char, int> card_counts;
    for (char card : hand) card_counts[card]++;

    if (card_counts.size() == 5) {
        if (card_counts['J'] == 1) return ONE_PAIR;
        return HIGH_CARD;
    }

    if (card_counts.size() == 4) {
        if (card_counts['J'] == 1 || card_counts['J'] == 2) return THREE_OF_A_KIND;
        return ONE_PAIR;
    }

    if (card_counts.size() == 3) {
        for (auto cardCount : card_counts) {
            if (cardCount.second == 3) {
                if (cardCount.first == 'J' || card_counts['J'] == 1) return FOUR_OF_A_KIND;
                return THREE_OF_A_KIND;
            }

            if (cardCount.second == 2) {
                if (cardCount.first == 'J' || card_counts['J'] == 2) return FOUR_OF_A_KIND;
                if (card_counts['J'] == 1) return FULL_HOUSE;
                return TWO_PAIR;
            }
        }
    }

    if (card_counts.size() == 2) {
        for (auto cardCount : card_counts) {
            if (cardCount.second == 4) {
                if (cardCount.first == 'J' || card_counts['J'] == 1) return FIVE_OF_A_KIND;
                return FOUR_OF_A_KIND;
            }

            if (cardCount.second == 3) {
                if (cardCount.first == 'J' || card_counts['J'] == 2) return FIVE_OF_A_KIND;
                return FULL_HOUSE;
            }
        }
    }

    if (card_counts.size() == 1) return FIVE_OF_A_KIND;
}

static bool compare_hand(string first_hand, string second_hand, bool p2) {
    auto _card_values = card_values;
    if (p2) _card_values['J'] = -1;

    const int value_first_hand = p2 ? value_hand_p2(first_hand) : value_hand_p1(first_hand);
    const int value_second_hand = p2 ? value_hand_p2(second_hand) : value_hand_p1(second_hand);

    if (value_first_hand > value_second_hand) return true;
    if (value_second_hand == value_first_hand) {
        for (int k = 0; k < first_hand.size(); k++) {
            char firstChar = first_hand[k];
            char secondChar = second_hand[k];

            if (_card_values[firstChar] > _card_values[secondChar]) return true;
            if (_card_values[firstChar] < _card_values[secondChar]) return false;
        }
    }

    return false;
}

void day7::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<pair<string, int>> hands;

        string current_line;
        while(getline(file, current_line)) {
            const int delimiter = current_line.find(" ");
            string hand = current_line.substr(0, delimiter);
            int bid = std::stoi(current_line.substr(delimiter + 1, current_line.size()));
            hands.push_back(pair<string, int>(hand, bid));
        }

        vector<pair<string, int>> hands_p1(hands.size());
        vector<pair<string, int>> hands_p2(hands.size());
        for (int i = 0; i < hands.size(); i++) {
            int worse_cards_p1 = 0;
            int worse_cards_p2 = 0;
            for (int j = 0; j < hands.size(); j++) {
                if (i == j) continue;
                if (compare_hand(hands[i].first, hands[j].first, false)) worse_cards_p1++;
                if (compare_hand(hands[i].first, hands[j].first, true)) worse_cards_p2++;
            }

            hands_p1[worse_cards_p1] = hands[i];
            hands_p2[worse_cards_p2] = hands[i];
        }

        int result_p1;
        for (int i = 0; i < hands_p1.size(); i++) result_p1 += hands_p1[i].second * (i + 1);

        int result_p2;
        for (int i = 0; i < hands_p2.size(); i++) result_p2 += hands_p2[i].second * (i + 1);

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}