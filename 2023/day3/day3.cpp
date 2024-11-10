//
// Created by Konstantin Kilbel on 05.12.23.
//

#include "../includes.h"
#include "day3.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

bool FindSymbols(const std::vector<std::string>& inputList, int startX, int startY, int length) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= length; ++j) {
            if (startY + i >= 0 && startY + i < inputList.size() &&
                startX + j >= 0 && startX + j < inputList[startY + i].size()) {
                if (!std::regex_match(std::string(1, inputList[startY + i][startX + j]), std::regex("[\\d\\.\\n]"))) {
                    return true;
                }
            }
        }
    }

    return false;
}

int CalculateGearRatio(const std::vector<std::string>& inputList, std::vector<std::vector<int>> inputNumbers, int startX, int startY) {
    int countAdjacentNumbers = 0;
    int gearRatio = 1;

    // loop all adjacent characters
    for (int i = startY - 1; i <= startY + 1; i++) {
        for (int j = startX - 1; j <= startX + 1; j++) {
            // check bound
            if (i >= 0 && i < inputList.size() && j >= 0 && j < inputList[i].size()) {
                // check if current char is digit
                if (isdigit(inputList[i][j])) {
                    // find it in the input numbers
                    for (int k = 0; k < inputNumbers[i].size(); k += 3) {
                        const auto start = inputNumbers[i][k];
                        const int end = start + inputNumbers[i][k + 1];

                        // number found
                        if (j >= start && j < end) {
                            countAdjacentNumbers++;
                            gearRatio *= inputNumbers[i][k + 2];
                            j = end;
                        }
                    }
                }
            }
        }
    }

    if (countAdjacentNumbers == 2) return gearRatio;

    return 0;
}

void day3::solve(std::string input) {
    std::ifstream file(input);

    if (file.is_open()) {
        std::vector<std::string> lines;
        std::vector<std::vector<int>> numPositions;

        // Save each line into a vector for easier access
        std::string currentLine;
        while (std::getline(file, currentLine)) lines.push_back(currentLine);

        // For every number we find, we store the position and the length (to make the box around it) and the value of the number to add it if it's valid
        for (size_t index = 0; index < lines.size(); ++index) {
            std::vector<int> tempVector;
            // Regex for finding all sequences of digits
            std::regex digitRegex("\\d+");
            auto digitIterator = std::sregex_iterator(lines[index].begin(), lines[index].end(), digitRegex);
            auto digitEnd = std::sregex_iterator();

            for (; digitIterator != digitEnd; ++digitIterator) {
                auto match = *digitIterator;
                tempVector.push_back(match.position());
                tempVector.push_back(match.length());
                tempVector.push_back(std::stoi(match.str()));
            }
            numPositions.push_back(tempVector);
        }

        int validNums = 0;
        int gearRatios = 0;

        // Adds all the valid numbers found
        for (size_t index = 0; index < numPositions.size(); ++index) {
            for (size_t i = 0; i < numPositions[index].size(); i += 3) {
                if (FindSymbols(lines, numPositions[index][i], static_cast<int>(index), numPositions[index][i + 1])) {
                    validNums += numPositions[index][i + 2];
                }
            }
        }

        // Calculates gear ratios for each gear
        for (int index = 0; index < lines.size(); ++index) {
            for (int i = 0; i < lines[index].size(); ++i) {
                if (lines[index][i] == '*') {
                    gearRatios += CalculateGearRatio(lines, numPositions, i, index);
                }
            }
        }

        std::cout << "Solution problem 1: " << validNums << std::endl;
        std::cout << "Solution problem 2: " << gearRatios << std::endl;

        file.close();
    } else {
        std::cout << "Can't open file" << std::endl;
    }
}