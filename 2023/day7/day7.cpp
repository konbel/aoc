//
// Created by Konstantin Kilbel on 07.12.23.
//

#include "../includes.h"
#include "day7.h"

#include <stdnoreturn.h>

constexpr int FIVE_OF_A_KIND = 7;
constexpr int FOUR_OF_A_KIND = 6;
constexpr int FULL_HOUSE = 5;
constexpr int THREE_OF_A_KIND = 4;
constexpr int TWO_PAIR = 3;
constexpr int ONE_PAIR = 2;
constexpr int HIGH_CARD = 1;

std::map<char, int> cardValues = {
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

int valueHandP1(std::string hand) {
    std::unordered_map<char, int> cardCounts;
    for (char card : hand) cardCounts[card]++;

    if (cardCounts.size() == 1) return FIVE_OF_A_KIND;
    if (cardCounts.size() == 5) return HIGH_CARD;

    int count2Pair = 0;
    int countTrips = 0;
    int countQuads = 0;
    for (auto i : cardCounts) {
        if (i.second == 2) count2Pair++;
        if (i.second == 3) countTrips++;
        if (i.second == 4) countQuads++;;
    }

    if (countQuads == 1) return FOUR_OF_A_KIND;
    if (countTrips == 1 && count2Pair == 1) return FULL_HOUSE;
    if (countTrips == 1) return THREE_OF_A_KIND;
    if (count2Pair == 2) return TWO_PAIR;
    if (count2Pair == 1) return ONE_PAIR;

    return HIGH_CARD;
}

int valueHandP2(std::string hand) {
    std::unordered_map<char, int> cardCounts;
    for (char card : hand) cardCounts[card]++;

    if (cardCounts.size() == 5) {
        if (cardCounts['J'] == 1) return ONE_PAIR;
        return HIGH_CARD;
    }

    if (cardCounts.size() == 4) {
        if (cardCounts['J'] == 1 || cardCounts['J'] == 2) return THREE_OF_A_KIND;
        return ONE_PAIR;
    }

    if (cardCounts.size() == 3) {
        for (auto cardCount : cardCounts) {
            if (cardCount.second == 3) {
                if (cardCount.first == 'J' || cardCounts['J'] == 1) return FOUR_OF_A_KIND;
                return THREE_OF_A_KIND;
            }

            if (cardCount.second == 2) {
                if (cardCount.first == 'J' || cardCounts['J'] == 2) return FOUR_OF_A_KIND;
                if (cardCounts['J'] == 1) return FULL_HOUSE;
                return TWO_PAIR;
            }
        }
    }

    if (cardCounts.size() == 2) {
        for (auto cardCount : cardCounts) {
            if (cardCount.second == 4) {
                if (cardCount.first == 'J' || cardCounts['J'] == 1) return FIVE_OF_A_KIND;
                return FOUR_OF_A_KIND;
            }

            if (cardCount.second == 3) {
                if (cardCount.first == 'J' || cardCounts['J'] == 2) return FIVE_OF_A_KIND;
                return FULL_HOUSE;
            }
        }
    }

    if (cardCounts.size() == 1) return FIVE_OF_A_KIND;
}

bool compareHand(std::string firstHand, std::string secondHand, bool p2) {
    auto _carValues = cardValues;
    if (p2) _carValues['J'] = -1;

    const int valueFirstHand = p2 ? valueHandP2(firstHand) : valueHandP1(firstHand);
    const int valueSecondHand = p2 ? valueHandP2(secondHand) : valueHandP1(secondHand);

    if (valueFirstHand > valueSecondHand) return true;
    if (valueSecondHand == valueFirstHand) {
        for (int k = 0; k < firstHand.size(); k++) {
            char firstChar = firstHand[k];
            char secondChar = secondHand[k];

            if (_carValues[firstChar] > _carValues[secondChar]) return true;
            if (_carValues[firstChar] < _carValues[secondChar]) return false;
        }
    }

    return false;
}

void day7::solve(std::string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        std::vector<std::pair<std::string, int>> hands;

        std::string currentLine;
        while(getline(file, currentLine)) {
            const int delimiter = currentLine.find(" ");
            std::string hand = currentLine.substr(0, delimiter);
            int bid = std::stoi(currentLine.substr(delimiter + 1, currentLine.size()));
            hands.push_back(std::pair<std::string, int>(hand, bid));
        }

        std::vector<std::pair<std::string, int>> handsP1(hands.size());
        std::vector<std::pair<std::string, int>> handsP2(hands.size());
        for (int i = 0; i < hands.size(); i++) {
            int worseCardsP1 = 0;
            int worseCardsP2 = 0;
            for (int j = 0; j < hands.size(); j++) {
                if (i == j) continue;
                if (compareHand(hands[i].first, hands[j].first, false)) worseCardsP1++;
                if (compareHand(hands[i].first, hands[j].first, true)) worseCardsP2++;
            }

            handsP1[worseCardsP1] = hands[i];
            handsP2[worseCardsP2] = hands[i];
        }

        int resultP1;
        for (int i = 0; i < handsP1.size(); i++) resultP1 += handsP1[i].second * (i + 1);

        int resultP2;
        for (int i = 0; i < handsP2.size(); i++) resultP2 += handsP2[i].second * (i + 1);

        std::cout << "Solution problem 1: " << resultP1 << std::endl;
        std::cout << "Solution problem 2: " << resultP2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}