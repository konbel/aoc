//
// Created by Konstantin Kilbel on 05.12.23.
//

#include "../includes.h";
#include "day2.h"
#include <vector>

std::pair<std::string, int> parseValue(std::string currentValueString) {
    const size_t i = currentValueString.find(" ");
    std::string digits = currentValueString.substr(0, i);
    return std::pair<std::string, int>(currentValueString.substr(i + 1, currentValueString.length()), std::stoi(currentValueString.substr(0, i)));
}

std::map<std::string, int> parseSet(std::string currentSetString) {
    std::map<std::string, int> currentSet;

    size_t i = currentSetString.find(",");
    while (i != std::string::npos) {
        const std::string currentValueString = currentSetString.substr(0, i);
        currentSet.insert(parseValue(currentValueString));
        currentSetString.erase(0, i + 2);
        i = currentSetString.find(",");
    }

    return currentSet;
}

std::vector<std::map<std::string, int>> parseLine(std::string currentLine) {
    std::vector<std::map<std::string, int>> currentSets;

    size_t i = currentLine.find(";");
    while (i != std::string::npos) {
        std::string currentSetString = currentLine.substr(0, i);
        currentSets.push_back(parseSet(currentSetString + ","));
        currentLine.erase(0, i + 2);
        i = currentLine.find(";");
    }

    return currentSets;
}

void logParsedLine(std::vector<std::map<std::string, int>> parsedLine) {
    for (int i = 0; i < parsedLine.size(); i++) {
        std::cout << "Set " << i + 1 << ": ";
        for (auto pair : parsedLine[i]) std::cout << pair.first << " " << pair.second << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void day2::solve(std::string input) {
    const int RED_CUBES = 12;
    const int GREEN_CUBES = 13;
    const int BLUE_CUBES = 14;
    const std::string COLORS[] = { "red", "green", "blue" };

    std::ifstream file(input);
    if (file.is_open()) {
        int resultP1 = 0;
        int resultP2 = 0;

        int lineCount = 1;
        std::string currentLine;
        while (std::getline(file, currentLine)) {
            currentLine.erase(0, currentLine.find(":") + 2);

            auto parsedLine = parseLine(currentLine + ";");

            bool isLinePossible = true;

            int maxRed = std::numeric_limits<int>::min();
            int maxGreen = std::numeric_limits<int>::min();
            int maxBlue = std::numeric_limits<int>::min();
            for (int i = 0; i < parsedLine.size(); i++) {
                // check if number is smaller than the given number
                if (parsedLine[i]["red"] > RED_CUBES) isLinePossible = false;
                if (parsedLine[i]["green"] > GREEN_CUBES) isLinePossible = false;
                if (parsedLine[i]["blue"] > BLUE_CUBES) isLinePossible = false;

                // get the minimum number of each color
                if (parsedLine[i]["red"] != 0 && parsedLine[i]["red"] > maxRed) maxRed = parsedLine[i]["red"];
                if (parsedLine[i]["green"] != 0 && parsedLine[i]["green"] > maxGreen) maxGreen = parsedLine[i]["green"];
                if (parsedLine[i]["blue"] != 0 && parsedLine[i]["blue"] > maxBlue) maxBlue = parsedLine[i]["blue"];
            }
            if (isLinePossible) resultP1 += lineCount;
            int power = maxRed * maxGreen * maxBlue;
            resultP2 += power;

            lineCount++;
        }

        std::cout << "Solution problem 1: " << resultP1 << std::endl;
        std::cout << "Solution problem 2: " << resultP2 << std::endl;
    } else std::cout << "Can't open file" << std::endl;
}