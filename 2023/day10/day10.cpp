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

void day10::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<string> lines;

        // read input file
        string current_line;
        while (getline(file, current_line)) lines.push_back(current_line);

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
        vector<int> current_position = start;
        vector<int> last_position = start;

        // find first direction
        bool found = false;
        if (start[1] - 1 >= 0 && pipes.contains(lines[start[1] - 1][start[0]])) { // top
            char pipe = lines[start[1] - 1][start[0]];
            if (pipe == '7' || pipe == 'F' || pipe == '|') {
                current_position[1]--;
                found = true;
            }
        }

        if (!found && start[1] + 1 < lines.size() && pipes.contains(lines[start[1] + 1][start[0]])) { // bottom
            char pipe = lines[start[1] + 1][start[0]];
            if (pipe == 'J' || pipe == 'L' || pipe == '|') {
                current_position[1]++;
                found = true;
            }
        }

        if (!found && start[0] - 1 >= 0 && pipes.contains(lines[start[1]][start[0] - 1])) { // left
            char pipe = lines[start[1]][start[0] - 1];
            if (pipe == 'F' || pipe == 'L' || pipe == '-') {
                current_position[0]--;
                found = true;
            }
        }

        if (!found && start[0] + 1 < lines[start[1]].size() && pipes.contains(lines[start[1]][start[0] + 1])) { // right
            char pipe = lines[start[1]][start[0] + 1];
            if (pipe == 'J' || pipe == '7' || pipe == '-') current_position[0]++;
        }

        // problem 1
        int steps;
        while (current_position != start) {
            steps++;
            char current_pipe = lines[current_position[1]][current_position[0]];
            if (pipes.contains(current_pipe)) {
                auto old_position = current_position;
                if (current_pipe == '|') { // vertical movement
                    if (last_position[1] < current_position[1]) current_position[1]++;
                    else current_position[1]--;
                } else if (current_pipe == '-') { // horizontal movement
                    if (last_position[0] < current_position[0]) current_position[0]++;
                    else current_position[0]--;
                } else { // corner movement
                    if (last_position[0] != current_position[0]) current_position[1] += pipes[current_pipe][1];
                    else current_position[0] += pipes[current_pipe][0];
                }
                last_position = old_position;
            }
        }

        // problem 2
        // no idea how to approach this problem, so I am waiting for other solutions to be available for inspiration

        std::cout << "Solution problem 1: " << (steps + 1) / 2 << std::endl;

    } else std::cout << "Can't open file" << std::endl;
}