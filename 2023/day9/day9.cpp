#include "../includes.h"
#include "day9.h"

static vector<int> parse_line(string line) {
    line += " ";

    vector<int> vec;

    string num;
    for (char c : line) {
        if (isdigit(c) || c == '-') num += c;
        else {
            vec.push_back(std::stoi(num));
            num.clear();
        }
    }

    return vec;
}

void day9::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<vector<int>> histories;

        string current_line;
        while (getline(file, current_line)) histories.push_back(parse_line(current_line));

        int result_p1 = 0;
        int result_p2 = 0;

        for (auto history : histories) {
            vector<vector<int>> differences;
            differences.push_back(history);

            while (true) {
                int last_index = differences.size() - 1;

                // calc differences
                vector<int> new_differences;
                for (int j = 0; j < differences[last_index].size() - 1; j++) new_differences.push_back(differences[last_index][j + 1] - differences[last_index][j]);
                differences.push_back(new_differences);
                last_index++;

                // check if all zero
                int zeroes = 0;
                for (int j : new_differences) {
                    if (j == 0) zeroes++;
                }

                if (zeroes == new_differences.size()) {
                    // extrapolate right
                    differences[last_index].push_back(0);
                    for (int j = differences.size() - 2; j >= 0; j--) differences[j].push_back(differences[j][differences[j].size() - 1] + differences[j + 1][differences[j + 1].size() - 1]);
                    result_p1 += differences[0][differences[0].size() - 1];

                    // extrapolate left
                    differences[last_index].insert(differences[last_index].begin(), 0);
                    for (int j = last_index - 1; j > 0; j--) differences[j - 1].insert(differences[j - 1].begin(), differences[j - 1][0] - differences[j][0]);
                    result_p2 += differences[0][0];
                    break;
                }
            }
        }

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}