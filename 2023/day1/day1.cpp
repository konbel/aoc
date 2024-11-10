#include "../includes.h"
#include "day1.h"

void day1::solve(std::string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        int resultP1 = 0;
        int resultP2 = 0;

        const std::string numberStrings[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

        // read file line by line
        std::string currentLine;
        while (std::getline(file, currentLine)) {
            std::map<int, std::string> currentNumbersP1;
            std::map<int, std::string> currentNumbersP2;

            // scan line for digits before replace
            for (int i = 0; i < currentLine.length(); i++) {
                if (isdigit(currentLine[i])) currentNumbersP1.insert(std::pair<int, std::string>(i, std::string {currentLine[i]}));
            }

            // replace number strings with digits
            for (int i = 0; i < 9; i++) {
                size_t index = currentLine.find(numberStrings[i], 0);
                while (index != std::string::npos) {
                    // replace number string with digit
                    currentLine[index + 1] = i + 1 + '0';
                    for (int j = index + 2; j < numberStrings[i].length() - 1; j++) currentLine[j] = ' ';

                    // try to find next sub string
                    index = currentLine.find(numberStrings[i], index + 1);
                }
            }

            // scan line for digits after replace
            for (int i = 0; i < currentLine.length(); i++) {
                if (isdigit(currentLine[i])) currentNumbersP2.insert(std::pair<int, std::string>(i, std::string {currentLine[i]}));
            }

            // join first and last digit string and add to total count
            resultP1 += std::stoi(currentNumbersP1.begin()->second + (--currentNumbersP1.end())->second);
            resultP2 += std::stoi(currentNumbersP2.begin()->second + (--currentNumbersP2.end())->second);
        }

        std::cout << "Solution problem 1: " << resultP1 << std::endl;
        std::cout << "Solution problem 2: " << resultP2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}