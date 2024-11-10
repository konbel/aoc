//
// Created by Konstantin Kilbel on 21.12.23.
//

#include "../includes.h"
#include "day21.h"
#include <assert.h>

vector<int> day21::addVector(const vector<int>& v1, const vector<int>& v2) {
    return {v1[0] + v2[0], v1[1] + v2[1]};
}

bool day21::checkBounds(const int& maxX, const int& maxY, const vector<int>& position) {
    if (position[0] >= 0 && position[0] < maxX && position[1] >= 0 && position[1] < maxY) return true;
    return false;
}

int day21::takeSteps(vector<string>& garden, const vector<int>& startPosition, const int& stepsToTake) {
    const vector<vector<int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    vector<vector<int>> moveQueue;
    vector<vector<int>> newMoves;
    moveQueue.push_back(startPosition);

    for (int i = 0; i < stepsToTake; i++) {
        while (!moveQueue.empty()) {
            vector position = moveQueue[0];

            for (vector<int> direction : directions) {
                vector<int> newPosition = addVector(position, direction);
                if (!checkBounds(garden[0].size(), garden.size(), newPosition)) continue;

                if (garden[newPosition[1]][newPosition[0]] == '.') {
                    newMoves.push_back(newPosition);
                    garden[newPosition[1]][newPosition[0]] = 'O';
                }
            }

            garden[position[1]][position[0]] = '.';
            moveQueue.erase(moveQueue.begin(), moveQueue.begin() + 1);
        }

        moveQueue = newMoves;
        newMoves.clear();
    }

    int countReached = 0;
    for (const string& l : garden) {
        for (const char& c : l) {
            if (c == 'O') countReached++;
        }
    }

    return countReached;
}
long long day21::calcSteps(vector<string>& garden, const vector<int>& startPosition, const int& stepsToTake) {
    const vector<vector<int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    assert(garden.size() == garden[0].size());
    const int size = garden.size();

    assert(stepsToTake % size == size / 2);

    const long gridWidth = stepsToTake / size - 1;
    const long odd = (gridWidth / 2 * 2 + 1) * (gridWidth / 2 * 2 + 1);
    const long even = ((gridWidth + 1) / 2 * 2) * ((gridWidth + 1) / 2 * 2);

    vector<string> gardenCopy = garden;
    const int odd_points = takeSteps(gardenCopy, startPosition, size * 2 + 1) * 2;

    gardenCopy = garden;
    const int event_points = takeSteps(gardenCopy, startPosition, size * 2) * 2;

    gardenCopy = garden;
    const int corner_t = takeSteps(gardenCopy, {startPosition[0], size - 1}, size - 1);

    gardenCopy = garden;
    const int corner_r = takeSteps(gardenCopy, {0, startPosition[1]}, size - 1);

    gardenCopy = garden;
    const int corner_b = takeSteps(gardenCopy, {startPosition[0], 0}, size - 1);

    gardenCopy = garden;
    const int corner_l = takeSteps(gardenCopy, {size - 1, startPosition[1]}, size - 1);


    gardenCopy = garden;
    const int small_tr = takeSteps(gardenCopy, {0, size - 1}, size / 2 - 1);

    gardenCopy = garden;
    const int small_tl = takeSteps(gardenCopy, {size - 1, size - 1 }, size / 2 - 1);

    gardenCopy = garden;
    const int small_br = takeSteps(gardenCopy, {0, 0}, size / 2 - 1);

    gardenCopy = garden;
    const int small_bl = takeSteps(gardenCopy, {size - 1, 0}, size / 2 - 1);


    gardenCopy = garden;
    const int large_tr = takeSteps(gardenCopy, {0, size - 1}, size * 3 / 2 - 1);

    gardenCopy = garden;
    const int large_tl = takeSteps(gardenCopy, {size - 1, size - 1 }, size * 3 / 2 - 1);

    gardenCopy = garden;
    const int large_br = takeSteps(gardenCopy, {0, 0}, size * 3 / 2 - 1);

    gardenCopy = garden;
    const int large_bl = takeSteps(gardenCopy, {size - 1, 0}, size * 3 / 2 - 1);

    const long long result = odd * odd_points +
         even * event_points +
         corner_t + corner_r + corner_b + corner_l +
         (gridWidth + 1) * (small_tr + small_tl + small_br + small_bl) +
         gridWidth * (large_tl + large_tr + large_bl + large_br);

    return result;
}

void day21::solve(const string& input) {
    std::ifstream file(input);
    if (file.is_open()) {
        vector<string> garden;
        vector<int> startPosition;

        string line;
        while (getline(file, line)) {
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == 'S') {
                    startPosition = {i, static_cast<int>(garden.size())};
                    line[i] = '.';
                    break;
                }
            }
            garden.push_back(line);
        }

        const int resultP1 = takeSteps(garden, startPosition, 64);
        const long long resultP2 = calcSteps(garden, startPosition, 26501365); // 594115391548176

        cout << "Solution problem 1: " << resultP1 << endl;
        cout << "Solution problem 2: " << resultP2 << endl;

    } else cout << "Can't open file" << endl;
}