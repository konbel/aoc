//
// Created by Konstantin Kilbel on 19.12.23.
//

#include "../includes.h"
#include "day19.h"
#include <unordered_map>

using day19::Condition;
using day19::Function;
using day19::Input;

long day19::sumCountAccepted(map<string, Function>& functions, std::unordered_map<char, std::pair<int, int>>& ranges, const std::string& name) {
    if (name == "R") return 0;
    if (name == "A") {
        long produkt = 1;
        for (const auto& range : ranges) produkt *= range.second.second - range.second.first + 1;
        return produkt;
    }

    const vector<Condition>& conditions = functions[name].conditions;
    const string exitCase = functions[name].exitCase;

    long total = 0;
    bool didBreak = false;
    for (const Condition& condition : conditions) {
        char key;
        if (condition.variable == 0) key = 'x';
        else if (condition.variable == 1) key = 'm';
        else if (condition.variable == 2) key = 'a';
        else if (condition.variable == 3) key = 's';

        int low = ranges[key].first;
        int high = ranges[key].second;

        std::pair<int, int> T;
        std::pair<int, int> F;

        if (condition.operation == '<') {
            T = std::pair(low, condition.compareTo - 1);
            F = std::pair(condition.compareTo, high);
        } else {
            T = std::pair(condition.compareTo + 1, high);
            F = std::pair(low, condition.compareTo);
        }

        if (T.first <= T.second) {
            auto newRanges2 = std::unordered_map{ranges};
            newRanges2[key] = T;
            total += sumCountAccepted(functions, newRanges2, condition.match);
        }

        if (F.first <= F.second) {
            ranges[key] = F;
        } else {
            didBreak = true;
            break;
        }
    }

    if (!didBreak) total += sumCountAccepted(functions, ranges, exitCase);

    return total;
}


string day19::testInput(const Input& input, const Function& function) {
    for (const Condition& condition : function.conditions) {
        int value = 0;
        if (condition.variable == 0) value = input.x;
        else if (condition.variable == 1) value = input.m;
        else if (condition.variable == 2) value = input.a;
        else if (condition.variable == 3) value = input.s;

        if (condition.operation == '>') {
            if (value > condition.compareTo) return condition.match;
        } else {
            if (value < condition.compareTo) return condition.match;
        }
    }

    return function.exitCase;
}

Function day19::parseFunction(string& line) {
    Function function;

    const int seperator = line.find('{');
    function.name = line.substr(0, seperator);
    line.erase(0, seperator + 1);

    line.erase(line.size() - 1, 1);
    for (int i = line.size() - 1; i >= 0; i--) {
        if (line[i] == ',') break;
        function.exitCase += line[i];
    }
    std::reverse(function.exitCase.begin(), function.exitCase.end());
    line.erase(line.size() - function.exitCase.size() - 1, function.exitCase.size() + 1);

    vector<Condition> conditions;
    while (!line.empty()) {
        string conditionString;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') break;
            conditionString += line[i];
        }

        Condition condition;
        if (conditionString[0] == 'x') condition.variable = 0;
        else if (conditionString[0] == 'm') condition.variable = 1;
        else if (conditionString[0] == 'a') condition.variable = 2;
        else if (conditionString[0] == 's') condition.variable = 3;

        condition.operation = conditionString[1];

        string compareString;
        for (int j = 2; j < conditionString.size(); j++) {
            if (conditionString[j] == ':') break;
            compareString += conditionString[j];
        }
        condition.compareTo = std::stoi(compareString);

        for (int j = conditionString.size() - 1; j >= 0; j--) {
            if (conditionString[j] == ':') break;
            condition.match += conditionString[j];
        }
        std::reverse(condition.match.begin(), condition.match.end());

        conditions.push_back(condition);
        line.erase(0, conditionString.size() + 1);
    }

    function.conditions = conditions;

    return function;
}

Input day19::parseInput(string& line) {
    line.erase(0, 1);
    line.erase(line.size() - 1, 1);

    Input input;

    while (!line.empty()) {
        string variableString;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') break;
            variableString += line[i];
        }

        string number;
        for (int i = 2; i < variableString.size(); i++) number += variableString[i];

        if (variableString[0] == 'x') input.x = std::stoi(number);
        else if (variableString[0] == 'm') input.m = std::stoi(number);
        else if (variableString[0] == 'a') input.a = std::stoi(number);
        else if (variableString[0] == 's') input.s = std::stoi(number);

        line.erase(0, variableString.size() + 1);
    }

    return input;
}

void day19::solve(const string& input) {
    std::ifstream file(input);
    if (file.is_open()) {
        int resultP1 = 0;

        bool finishedParsingFunctions = false;

        map<string, Function> functions;
        vector<Input> inputs;

        string line;
        while (getline(file, line)) {
            if (line.empty()) {
                finishedParsingFunctions = true;
                continue;
            }

            if (!finishedParsingFunctions) {
                Function function = parseFunction(line);
                functions.insert(std::pair(function.name, function));
            } else inputs.push_back(parseInput(line));
        }

        for (int i = 0; i < inputs.size(); i++) {
            const Input input = inputs[i];

            string result = testInput(input, functions["in"]);
            while (true) {
                result = testInput(input, functions[result]);
                if (result == "A" || result == "R") break;
            }

            if (result == "A") resultP1 += input.x + input.m + input.a + input.s;
        }

        std::unordered_map<char, std::pair<int, int>> ranges;
        ranges['x'] = std::pair(1, 4000);
        ranges['m'] = std::pair(1, 4000);
        ranges['a'] = std::pair(1, 4000);
        ranges['s'] = std::pair(1, 4000);
        long long resultP2 = sumCountAccepted(functions, ranges, "in");

        printf("Solution problem 1: %d\n", resultP1);

        cout << "Solution problem 2: " << resultP2 << endl;

        file.close();
    } else cout << "Can't open file" << endl;
}