//
// Created by Konstantin Kilbel on 08.12.23.
//

#include "../includes.h"
#include "day8.h"

std::pair<std::string, std::vector<std::string>> parseNode(std::string line) {
    std::pair<std::string, std::vector<std::string>> node;
    int seperator = line.find(" ");
    node.first = line.substr(0, seperator);

    std::vector<std::string> lr;
    lr.push_back(line.substr(seperator + 4, 3));
    lr.push_back(line.substr(seperator + 9, 3));
    node.second = lr;

    return node;
}

std::vector<int> getSteps(std::vector<std::pair<std::string, std::vector<std::string>>> currentNodes, std::string instructions, std::map<std::string, std::vector<std::string>> nodes) {
    std::vector<int> stepsNeeded;
    for (auto& node : currentNodes) {
        int steps = 0;
        while (node.first[node.first.size() - 1] != 'Z') {
            for (int i = 0; i < instructions.size(); i++) {
                steps++;
                if (instructions[i] == 'R') {
                    auto temp = nodes.find(node.second[1]);
                    node.first = temp->first;
                    node.second = temp->second;
                } else if (instructions[i] == 'L') {
                    auto temp = nodes.find(node.second[0]);
                    node.first = temp->first;
                    node.second = temp->second;
                }
                if (node.first[node.first.size() - 1] == 'Z') break;
            }
        }
        stepsNeeded.push_back(steps);
    }

    return stepsNeeded;
}

void day8::solve(std::string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        // get directions
        std::string instructions;
        getline(file, instructions);

        std::string currentLine;
        getline(file, currentLine);

        // get nodes
        std::map<std::string, std::vector<std::string>> nodes;
        while (getline(file, currentLine)) nodes.insert(parseNode(currentLine));

        // problem 1
        auto currentNode = nodes.find("AAA");
        int resultP1 = 0;
        // while (currentNode->first != "ZZZ") {
        //     for (int i = 0; i < instructions.size(); i++) {
        //         resultP1++;
        //         if (instructions[i] == 'R') currentNode = nodes.find(currentNode->second[1]);
        //         else if (instructions[i] == 'L') currentNode = nodes.find(currentNode->second[0]);
        //         if (currentNode->first == "ZZZ") break;
        //     }
        // }

        // problem 2
        std::vector<std::pair<std::string, std::vector<std::string>>> currentNodes;
        for (auto node : nodes) {
            if (node.first[node.first.size() - 1] == 'A') currentNodes.push_back(node);
        }

        int resultP2 = 0;
        std::vector<int> stepsNeeded(currentNodes.size());

        // works but slow af, no time for different solution
        while (resultP2 == 0) {
            for (int j = 0; j < currentNodes.size(); j++) {
                auto& node = currentNodes[j];
                int steps = 0;
                do {
                    for (int i = 0; i < instructions.size(); i++) {
                        steps++;
                        if (instructions[i] == 'R') {
                            auto temp = nodes.find(node.second[1]);
                            node.first = temp->first;
                            node.second = temp->second;
                        } else if (instructions[i] == 'L') {
                            auto temp = nodes.find(node.second[0]);
                            node.first = temp->first;
                            node.second = temp->second;
                        }
                        if (node.first[node.first.size() - 1] == 'Z') break;
                    }
                } while (node.first[node.first.size() - 1] != 'Z');
                stepsNeeded[j] = stepsNeeded[j] + steps;

                int matches = 1;
                for (int i = 1; i < stepsNeeded.size(); i++) {
                    if (stepsNeeded[0] == stepsNeeded[1]) matches++;
                }
                if (matches == stepsNeeded.size()) {
                    resultP2 = stepsNeeded[0];
                    break;
                }
            }
        }

        std::cout << "Solution problem 1: " << resultP1 << std::endl;
        std::cout << "Solution problem 2: " << resultP2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}