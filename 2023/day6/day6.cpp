//
// Created by Konstantin Kilbel on 07.12.23.
//

#include "../includes.h"
#include "day6.h"

#include <string>

void day6::solve(std::string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        std::vector<int> times;
        std::vector<int> distances;

        // get times
        std::string currentLine;
        getline(file, currentLine);
        currentLine += " ";

        std::string currentNumber;
        for (char c : currentLine) {
            if (isdigit(c)) currentNumber += c;
            else {
                if (!currentNumber.empty()) {
                    times.push_back(std::stoi(currentNumber));
                    currentNumber.clear();
                }
            }
        }

        // get distances
        getline(file, currentLine);
        currentLine += " ";

        for (char c : currentLine) {
            if (isdigit(c)) currentNumber += c;
            else {
                if (!currentNumber.empty()) {
                    distances.push_back(std::stoi(currentNumber));
                    currentNumber.clear();
                }
            }
        }

        // solve problem 1
        int resultP1 = 1;
        for (int i = 0; i < times.size(); i++) {
            int countWins = 0;
            for (int t = 0; t < times[i]; t++) if (t * (times[i] - t) > distances[i]) countWins++;
            resultP1 *= countWins;
        }

        // solve problem 2
        std::string newTime;
        for (int t : times) newTime += std::to_string(t);
        long time = std::stol(newTime);

        std::string newDistance;
        for (int d : distances) newDistance += std::to_string(d);
        long distance = std::stol(newDistance);

        int resultP2 = 0;
        for (long i = 0; i < time; i++) {
            if (i * (time - i) > distance) resultP2++;
        }

        std::cout << "Solution problem 1: " << resultP1 << std::endl;
        std::cout << "Solution problem 2: " << resultP2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}
