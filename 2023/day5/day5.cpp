//
// Created by Konstantin Kilbel on 06.12.23.
//

#include "../includes.h"
#include "day5.h"

std::vector<long> parseNumbers(std::string line) {
    std::vector<long> numbers;
    std::string currentNumber;
    for (long i = 0; i < line.size(); i++) {
        if (isdigit(line[i])) currentNumber += line[i];
        else {
            if (!currentNumber.empty()) {
                numbers.push_back(std::stol(currentNumber));
                currentNumber = "";
            }
        }
    }
    numbers.push_back(std::stoi(currentNumber));

    return numbers;
}

void day5::solve(std::string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        // get seeds
        std::string seedsString;
        getline(file, seedsString);

        seedsString.erase(0, seedsString.find(":") + 2);
        if (seedsString[0] == ' ') seedsString.erase(0, 1);

        std::vector<long> seeds = parseNumbers(seedsString);

        // parse maps
        std::vector<std::vector<std::vector<long>>> maps;

        std::string currentLine;
        std::vector<std::vector<long>> currentMap;
        while (getline(file, currentLine)) {
            if (isalpha(currentLine[0])) continue;
            if (currentLine.empty()) {
                if (!currentMap.empty()) {
                    maps.push_back(currentMap);
                    currentMap.clear();
                }
                continue;
            }

            currentMap.push_back(parseNumbers(currentLine));
        }
        maps.push_back(currentMap);

        // evaluate mappings - problem 1
        std::vector<long> resultP1 = seeds;
        for (long& seed : resultP1) {
            for (auto map : maps) {
                bool seedSet = false;
                for (auto mapping : map) {
                    if (seed >= mapping[1] && seed <= mapping[1] + mapping[2]) {
                        seed = mapping[0] + (seed - mapping[1]);
                        seedSet = true;
                    }

                    if (seedSet) break;
                }
            }
        }

        // evaluate mappings - problem 2
        std::vector<std::vector<long>> ranges;
        for (int i = 0; i < seeds.size(); i += 2) {
            std::vector<long> currentRange;
            for (int j = 0; j < seeds[i + 1]; j++) currentRange.push_back(seeds[i] + j);
            ranges.push_back(currentRange);
        }

        // parallization refused to work so this is really slow
        for (std::size_t i = 0; i < ranges.size(); ++i) {
            auto& range = ranges[i];
            for (auto& seed : range) {
                for (auto map : maps) {
                    bool seedSet = false;
                    for (auto mapping : map) {
                        if (seed >= mapping[1] && seed <= mapping[1] + mapping[2]) {
                            seed = mapping[0] + (seed - mapping[1]);
                            seedSet = true;
                        }

                        if (seedSet) break;
                    }
                }
            }
        }

        std::vector<long> resultP2;
        for (auto range : ranges) {
            for (auto result : range) resultP2.push_back(result);
        }

        std::cout << "Solution problem 1: " << std::ranges::min(resultP1) << std::endl;
        std::cout << "Solution problem 2: " << std::ranges::min(resultP2) << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}