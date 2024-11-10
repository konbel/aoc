//
// Created by Konstantin Kilbel on 06.12.23.
//

#include "../includes.h"
#include "day4.h"

void day4::solve(std::string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        int resultP1, resultP2;
        std::vector<int> matches;

        // read file line by line
        std::string currentLine;
        int lines;
        while (getline(file, currentLine)) {
            lines++;

            // remove prefix
            currentLine.erase(0, currentLine.find(":") + 2);

            // split string by seperator
            int seperator = currentLine.find("|");
            std::string winningNumbersString = currentLine.substr(0, seperator);
            currentLine.erase(0, seperator + 2);

            // remove leading space
            if (currentLine[0] == ' ') currentLine.erase(0, 1);
            if (winningNumbersString[0] == ' ') winningNumbersString.erase(0, 1);

            // add trailing space
            currentLine += " ";
            winningNumbersString += " ";

            // parse winning numbers
            std::vector<int> winningNumbers;
            std::string n;
            for (int i = 0; i < currentLine.size(); i++) {
                if (isdigit(winningNumbersString[i])) n += winningNumbersString[i];
                else {
                    if (!n.empty()) {
                        winningNumbers.push_back(std::stoi(n));
                        n = std::string();
                    }
                }
            }

            // parse current numbers
            std::vector<int> currentNumbers;
            for (int i = 0; i < currentLine.size(); i++) {
                if (isdigit(currentLine[i])) n += currentLine[i];
                else {
                    if (!n.empty()) {
                        currentNumbers.push_back(std::stoi(n));
                        n = std::string();
                    }
                }
            }

            // compare current and winning numbers
            int currentResult = 0;
            int currentMatches = 0;
            for (int c : currentNumbers) {
                for (int w : winningNumbers) {
                    if (c == w) {
                        currentMatches++;
                        if (currentResult == 0) currentResult = 1;
                        else currentResult *= 2;
                    }
                }
            }

            matches.push_back(currentMatches);

            resultP1 += currentResult;
        }

        // calculate number of copies
        std::vector<int> repeatLineCount(lines);
        for (int& i : repeatLineCount) i = 1;

        for (int i = 0; i < repeatLineCount.size(); i++) {
            for (int j = 0; j < repeatLineCount[i]; j++) {
                for (int k = 1; k <= matches[i]; k++) {
                    repeatLineCount[i + k]++;
                }
            }
        }

        // sum number of copies
        for (int i : repeatLineCount) resultP2 += i;

        std::cout << "Solution problem 1: " << resultP1 << std::endl;
        std::cout << "Solution problem 2: " << resultP2 <<  std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}