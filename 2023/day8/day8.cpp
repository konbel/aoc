#include "../includes.h"
#include "day8.h"

static pair<string, vector<string>> parse_node(string line) {
    pair<string, vector<string>> node;
    int seperator = line.find(" ");
    node.first = line.substr(0, seperator);

    vector<string> lr;
    lr.push_back(line.substr(seperator + 4, 3));
    lr.push_back(line.substr(seperator + 9, 3));
    node.second = lr;

    return node;
}

static vector<int> get_steps(vector<pair<string, vector<string>>> current_nodes, string instructions, map<string, vector<string>> nodes) {
    vector<int> steps_needed;
    for (auto& node : current_nodes) {
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
        steps_needed.push_back(steps);
    }

    return steps_needed;
}

void day8::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        // get directions
        string instructions;
        getline(file, instructions);

        string current_line;
        getline(file, current_line);

        // get nodes
        map<string, vector<string>> nodes;
        while (getline(file, current_line)) nodes.insert(parse_node(current_line));

        // problem 1
        auto currentNode = nodes.find("AAA");
        int result_p1 = 0;
        while (currentNode->first != "ZZZ") {
            for (int i = 0; i < instructions.size(); i++) {
                result_p1++;
                if (instructions[i] == 'R') currentNode = nodes.find(currentNode->second[1]);
                else if (instructions[i] == 'L') currentNode = nodes.find(currentNode->second[0]);
                if (currentNode->first == "ZZZ") break;
            }
        }

        // problem 2
        vector<pair<string, vector<string>>> current_nodes;
        for (auto node : nodes) {
            if (node.first[node.first.size() - 1] == 'A') current_nodes.push_back(node);
        }

        int result_p2 = 0;
        vector<int> steps_needed(current_nodes.size());

        // works but slow af, no time for different solution
        while (result_p2 == 0) {
            for (int j = 0; j < current_nodes.size(); j++) {
                auto& node = current_nodes[j];
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
                steps_needed[j] = steps_needed[j] + steps;

                int matches = 1;
                for (int i = 1; i < steps_needed.size(); i++) {
                    if (steps_needed[0] == steps_needed[1]) matches++;
                }
                if (matches == steps_needed.size()) {
                    result_p2 = steps_needed[0];
                    break;
                }
            }
        }

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}