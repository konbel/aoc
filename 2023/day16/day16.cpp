//
// Created by Konstantin Kilbel on 16.12.23.
//

#include "../includes.h"
#include "day16.h"

int day16::shootRay(const vector<string>& grid, const vector<int>& startPosition, const vector<int>& direction) {
    std::map<vector<int>, bool> energizedFields;

    std::map<vector<int>, int> raysSeen;
    vector<Ray> rays;

    Ray startRay = Ray();
    startRay.position = startPosition;
    startRay.direction = direction;
    rays.push_back(startRay);
    raysSeen[startRay.position] = true;

    while (!rays.empty()) {
        while (true) {
            rays[0].position[0] += rays[0].direction[0];
            rays[0].position[1] += rays[0].direction[1];
            if (rays[0].position[0] < 0 || rays[0].position[0] >= grid[0].size() || rays[0].position[1] < 0 || rays[0].position[1] >= grid.size()) break;
            energizedFields[rays[0].position] = true;

            const char currentObject = grid[rays[0].position[1]][rays[0].position[0]];
            if (currentObject == '.') continue;

            if (currentObject == '/') {
                if (rays[0].direction[0] == 1) rays[0].direction = {0, -1}; // coming from left
                else if (rays[0].direction[0] == -1) rays[0].direction = {0, 1}; // coming from right
                else if (rays[0].direction[1] == 1) rays[0].direction = {-1, 0}; // coming from top
                else if (rays[0].direction[1] == -1) rays[0].direction = {1, 0}; // coming from bottom
            } else if (currentObject == '\\') {
                if (rays[0].direction[0] == 1) rays[0].direction = {0, 1}; // coming from left
                else if (rays[0].direction[0] == -1) rays[0].direction = {0, -1}; // coming from right
                else if (rays[0].direction[1] == 1) rays[0].direction = {1, 0}; // coming from top
                else if (rays[0].direction[1] == -1) rays[0].direction = {-1, 0}; // coming from bottom
            }

            if (currentObject == '-') {
                if (rays[0].direction[1] == 0) continue;

                Ray leftRay = Ray();
                leftRay.position = rays[0].position;
                leftRay.direction = {-1, 0};

                if (raysSeen[leftRay.position] < 2) {
                    rays.push_back(leftRay);
                    raysSeen[leftRay.position]++;
                }

                Ray rightRay = Ray();
                rightRay.position = rays[0].position;
                rightRay.direction = {1, 0};

                if (raysSeen[rightRay.position] < 2) {
                    rays.push_back(rightRay);
                    raysSeen[rightRay.position]++;
                }

                break;
            }

            if (currentObject == '|') {
                if (rays[0].direction[0] == 0) continue;

                Ray topRay = Ray();
                topRay.position = rays[0].position;
                topRay.direction = {0, -1};

                if (raysSeen[topRay.position] < 2) {
                    rays.push_back(topRay);
                    raysSeen[topRay.position]++;
                }

                Ray downRay = Ray();
                downRay.position = rays[0].position;
                downRay.direction = {0, 1};

                if (raysSeen[downRay.position] < 2) {
                    rays.push_back(downRay);
                    raysSeen[downRay.position]++;
                }

                break;
            }
        }
        rays.erase(rays.begin(), rays.begin() + 1);
    }

    return energizedFields.size();
}

void day16::solve(const string& input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<string> grid;

        string line;
        while (getline(file, line)) grid.push_back(line);

        const int resultP1 = shootRay(grid, {-1, 0}, {1, 0});

        vector<int> results;

        for (int i = 0; i < grid[0].size(); i++) results.push_back(shootRay(grid, {i, -1}, {0, 1})); // top row
        for (int i = 0; i < grid[0].size(); i++) results.push_back(shootRay(grid, {i, static_cast<int>(grid.size())}, {0, -1})); // bottom row

        for (int i = 0; i < grid.size(); i++) results.push_back(shootRay(grid, {-1, i}, {1, 0})); // left row
        for (int i = 0; i < grid.size(); i++) results.push_back(shootRay(grid, {static_cast<int>(grid[0].size()), i}, {-1, 0})); // left row

        cout << "Solution problem 1: " << resultP1 << endl;
        cout << "Solution problem 2: " << std::ranges::max(results) << endl;

        file.close();
    } else cout << "Can't open file" << endl;
}
