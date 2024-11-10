//
// Created by Konstantin Kilbel on 18.12.23.
//

#include "../includes.h"
#include "day18.h"
#include <sstream>

vector<vector<long>> day18::calculateCorners(const vector<std::pair<char, int>>& steps) {
    vector<vector<long>> corners;
    corners.push_back({0, 0});

    for (int i = 0; i < steps.size(); i++) {
        int dx = 0;
        int dy = 0;

        if (steps[i].first == 'R') dx = steps[i].second;
        else if (steps[i].first == 'L') dx = -steps[i].second;
        else if (steps[i].first == 'U') dy = -steps[i].second;
        else if (steps[i].first == 'D') dy = steps[i].second;

        const int lastX = corners[corners.size() - 1][0];
        const int lastY = corners[corners.size() - 1][1];

        corners.push_back({lastX + dx, lastY + dy});
    }

    return corners;
}

long day18::calculateSurfaceArea(const vector<std::pair<char, int>>& steps) {
    const vector<vector<long>> corners = calculateCorners(steps);

    long b = 0;
    for (int i = 0; i < steps.size(); i++) b += steps[i].second;

    long area = 0;
    for (int i = 1; i < corners.size(); i++) area += corners[i][0] * (corners[(i + 1) % corners.size()][1] - corners[i - 1][1]);
    area = std::abs(area) / 2;

    const long i = area - std::floor(b / 2) + 1;

    return i + b;
}

void day18::solve(const string& input) {
    std::ifstream file(input);
    if (file.is_open()) {
        vector<std::pair<char, int>> stepsP1;
        vector<std::pair<char, int>> stepsP2;

        string line;
        while (getline(file, line)) {
            char directionP1 = line[0];
            line.erase(0, 2);

            const int seperator = line.find(" ");
            int stepsSmall = std::stoi(line.substr(0, seperator));
            stepsP1.push_back(std::pair(directionP1, stepsSmall));

            string hexString = line.substr(line.size() - 7, 6);

            int directionInt = hexString[hexString.size() - 1] - '0';
            char directionP2;
            if (directionInt == 0) directionP2 = 'R';
            else if (directionInt == 1) directionP2 = 'D';
            else if (directionInt == 2) directionP2 = 'L';
            else if (directionInt == 3) directionP2 = 'U';

            int stepsBig = 0;
            std::stringstream ss;
            ss << std::hex << hexString.substr(0, 5);
            ss >> stepsBig;

            stepsP2.push_back(std::pair(directionP2, stepsBig));
        }

        cout << "Solution problem 1: " << calculateSurfaceArea(stepsP1) << endl;
        cout << "Solution problem 2: " << calculateSurfaceArea(stepsP2) << endl;

        file.close();
    } else cout << "Can't open file" << endl;
}
