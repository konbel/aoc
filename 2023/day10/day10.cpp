//
// Created by Konstantin Kilbel on 10.12.23.
//

#include "../includes.h"
#include "day10.h"

map<char, vector<int>> pipes = {
    {'L', {1, -1}},
    {'J', {-1, -1}},
    {'7', {-1, 1}},
    {'F', {1, 1}},
    {'|', {}},
    {'-', {}},
};

void day10::solve(std::string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        vector<string> lines;

        // read input file
        string currentLine;
        while (getline(file, currentLine)) lines.push_back(currentLine);

        // locate start
        vector<int> start;
        for (int y = 0; y < lines.size(); y++) {
            int x = lines[y].find("S");
            if (x != string::npos) {
                start.push_back(x);
                start.push_back(y);
                break;
            }
        }

        // walk loop and count steps
        vector<int> currentPosition = start;
        vector<int> lastPosition = start;

        // find first direction
        bool found = false;
        if (start[1] - 1 >= 0 && pipes.contains(lines[start[1] - 1][start[0]])) { // top
            char pipe = lines[start[1] - 1][start[0]];
            if (pipe == '7' || pipe == 'F' || pipe == '|') {
                currentPosition[1]--;
                found = true;
            }
        }

        if (!found && start[1] + 1 < lines.size() && pipes.contains(lines[start[1] + 1][start[0]])) { // bottom
            char pipe = lines[start[1] + 1][start[0]];
            if (pipe == 'J' || pipe == 'L' || pipe == '|') {
                currentPosition[1]++;
                found = true;
            }
        }

        if (!found && start[0] - 1 >= 0 && pipes.contains(lines[start[1]][start[0] - 1])) { // left
            char pipe = lines[start[1]][start[0] - 1];
            if (pipe == 'F' || pipe == 'L' || pipe == '-') {
                currentPosition[0]--;
                found = true;
            }
        }

        if (!found && start[0] + 1 < lines[start[1]].size() && pipes.contains(lines[start[1]][start[0] + 1])) { // right
            char pipe = lines[start[1]][start[0] + 1];
            if (pipe == 'J' || pipe == '7' || pipe == '-') currentPosition[0]++;
        }

        // problem 1
        int steps;
        while (currentPosition != start) {
            steps++;
            char currentPipe = lines[currentPosition[1]][currentPosition[0]];
            if (pipes.contains(currentPipe)) {
                auto oldPosition = currentPosition;
                if (currentPipe == '|') { // vertical movement
                    if (lastPosition[1] < currentPosition[1]) currentPosition[1]++;
                    else currentPosition[1]--;
                } else if (currentPipe == '-') { // horizontal movement
                    if (lastPosition[0] < currentPosition[0]) currentPosition[0]++;
                    else currentPosition[0]--;
                } else { // corner movement
                    if (lastPosition[0] != currentPosition[0]) currentPosition[1] += pipes[currentPipe][1];
                    else currentPosition[0] += pipes[currentPipe][0];
                }
                lastPosition = oldPosition;
            }
        }

        // problem 2
        // no idea how to approach this problem, so I am waiting for other solutions to be available for inspiration

        cout << "Solution problem 1: " << (steps + 1) / 2 << endl;

    } else cout << "Can't open file" << endl;
}