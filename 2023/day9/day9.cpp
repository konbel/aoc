//
// Created by Konstantin Kilbel on 09.12.23.
//

#include "../includes.h"
#include "day9.h"

std::vector<int> day9::parseLine(std::string line) {
    line += " ";

    std::vector<int> vec;

    std::string num;
    for (char c : line) {
        if (isdigit(c) || c == '-') num += c;
        else {
            vec.push_back(std::stoi(num));
            num.clear();
        }
    }

    return vec;
}

void day9::solve(std::string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        std::vector<std::vector<int>> histories;

        std::string currentLine;
        while (getline(file, currentLine)) histories.push_back(parseLine(currentLine));

        int resultP1 = 0;
        int resultP2 = 0;

        for (auto history : histories) {
            std::vector<std::vector<int>> differences;
            differences.push_back(history);

            while (true) {
                int lastIndex = differences.size() - 1;

                // calc differences
                std::vector<int> newDifferences;
                for (int j = 0; j < differences[lastIndex].size() - 1; j++) newDifferences.push_back(differences[lastIndex][j + 1] - differences[lastIndex][j]);
                differences.push_back(newDifferences);
                lastIndex++;

                // check if all zero
                int zeroes = 0;
                for (int j : newDifferences) {
                    if (j == 0) zeroes++;
                }

                if (zeroes == newDifferences.size()) {
                    // extrapolate right
                    differences[lastIndex].push_back(0);
                    for (int j = differences.size() - 2; j >= 0; j--) differences[j].push_back(differences[j][differences[j].size() - 1] + differences[j + 1][differences[j + 1].size() - 1]);
                    resultP1 += differences[0][differences[0].size() - 1];

                    // extrapolate left
                    differences[lastIndex].insert(differences[lastIndex].begin(), 0);
                    for (int j = lastIndex - 1; j > 0; j--) differences[j - 1].insert(differences[j - 1].begin(), differences[j - 1][0] - differences[j][0]);
                    resultP2 += differences[0][0];
                    break;
                }
            }
        }

        std::cout << "Solution problem 1: " << resultP1 << std::endl;
        std::cout << "Solution problem 2: " << resultP2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}