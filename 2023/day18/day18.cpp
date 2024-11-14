#include "../includes.h"
#include "day18.h"
#include <sstream>

static vector<vector<long>> calculate_corners(const vector<pair<char, int>>& steps) {
    vector<vector<long>> corners;
    corners.push_back({0, 0});

    for (int i = 0; i < steps.size(); i++) {
        int dx = 0;
        int dy = 0;

        if (steps[i].first == 'R') dx = steps[i].second;
        else if (steps[i].first == 'L') dx = -steps[i].second;
        else if (steps[i].first == 'U') dy = -steps[i].second;
        else if (steps[i].first == 'D') dy = steps[i].second;

        const int last_x = corners[corners.size() - 1][0];
        const int last_y = corners[corners.size() - 1][1];

        corners.push_back({last_x + dx, last_y + dy});
    }

    return corners;
}

static long calculate_surface_area(const vector<pair<char, int>>& steps) {
    const vector<vector<long>> corners = calculate_corners(steps);

    long b = 0;
    for (int i = 0; i < steps.size(); i++) b += steps[i].second;

    long area = 0;
    for (int i = 1; i < corners.size(); i++) area += corners[i][0] * (corners[(i + 1) % corners.size()][1] - corners[i - 1][1]);
    area = std::abs(area) / 2;

    const long i = area - std::floor(b / 2) + 1;

    return i + b;
}

void day18::solve(const string& input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<pair<char, int>> steps_p1;
        vector<pair<char, int>> steps_p2;

        string line;
        while (getline(file, line)) {
            char directionP1 = line[0];
            line.erase(0, 2);

            const int seperator = line.find(" ");
            int steps_small = std::stoi(line.substr(0, seperator));
            steps_p1.push_back(pair(directionP1, steps_small));

            string hex_string = line.substr(line.size() - 7, 6);

            int direction_int = hex_string[hex_string.size() - 1] - '0';
            char direction_p2;
            if (direction_int == 0) direction_p2 = 'R';
            else if (direction_int == 1) direction_p2 = 'D';
            else if (direction_int == 2) direction_p2 = 'L';
            else if (direction_int == 3) direction_p2 = 'U';

            int steps_big = 0;
            std::stringstream ss;
            ss << std::hex << hex_string.substr(0, 5);
            ss >> steps_big;

            steps_p2.push_back(pair(direction_p2, steps_big));
        }

        std::cout << "Solution problem 1: " << calculate_surface_area(steps_p1) << std::endl;
        std::cout << "Solution problem 2: " << calculate_surface_area(steps_p2) << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}
