#include "../includes.h"
#include "day19.h"

struct Condition {
    int variable;
    char operation;
    int compare_to;
    string match;
};

struct Function {
    string name;
    vector<Condition> conditions;
    string exit_case;
};

struct Input {
    int x, m, a, s;
};

static long sum_count_accepted(map<string, Function>& functions, std::unordered_map<char, pair<int, int>>& ranges, const string& name) {
    if (name == "R") return 0;
    if (name == "A") {
        long produkt = 1;
        for (const auto& range : ranges) produkt *= range.second.second - range.second.first + 1;
        return produkt;
    }

    const vector<Condition>& conditions = functions[name].conditions;
    const string exit_case = functions[name].exit_case;

    long total = 0;
    bool did_break = false;
    for (const Condition& condition : conditions) {
        char key;
        if (condition.variable == 0) key = 'x';
        else if (condition.variable == 1) key = 'm';
        else if (condition.variable == 2) key = 'a';
        else if (condition.variable == 3) key = 's';

        int low = ranges[key].first;
        int high = ranges[key].second;

        pair<int, int> T;
        pair<int, int> F;

        if (condition.operation == '<') {
            T = pair(low, condition.compare_to - 1);
            F = pair(condition.compare_to, high);
        } else {
            T = pair(condition.compare_to + 1, high);
            F = pair(low, condition.compare_to);
        }

        if (T.first <= T.second) {
            auto new_ranges_2 = std::unordered_map{ranges};
            new_ranges_2[key] = T;
            total += sum_count_accepted(functions, new_ranges_2, condition.match);
        }

        if (F.first <= F.second) {
            ranges[key] = F;
        } else {
            did_break = true;
            break;
        }
    }

    if (!did_break) total += sum_count_accepted(functions, ranges, exit_case);

    return total;
}


static string test_input(const Input& input, const Function& function) {
    for (const Condition& condition : function.conditions) {
        int value = 0;
        if (condition.variable == 0) value = input.x;
        else if (condition.variable == 1) value = input.m;
        else if (condition.variable == 2) value = input.a;
        else if (condition.variable == 3) value = input.s;

        if (condition.operation == '>') {
            if (value > condition.compare_to) return condition.match;
        } else {
            if (value < condition.compare_to) return condition.match;
        }
    }

    return function.exit_case;
}

static Function parse_function(string& line) {
    Function function;

    const int seperator = line.find('{');
    function.name = line.substr(0, seperator);
    line.erase(0, seperator + 1);

    line.erase(line.size() - 1, 1);
    for (int i = line.size() - 1; i >= 0; i--) {
        if (line[i] == ',') break;
        function.exit_case += line[i];
    }
    std::reverse(function.exit_case.begin(), function.exit_case.end());
    line.erase(line.size() - function.exit_case.size() - 1, function.exit_case.size() + 1);

    vector<Condition> conditions;
    while (!line.empty()) {
        string condition_string;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') break;
            condition_string += line[i];
        }

        Condition condition;
        if (condition_string[0] == 'x') condition.variable = 0;
        else if (condition_string[0] == 'm') condition.variable = 1;
        else if (condition_string[0] == 'a') condition.variable = 2;
        else if (condition_string[0] == 's') condition.variable = 3;

        condition.operation = condition_string[1];

        string compare_string;
        for (int j = 2; j < condition_string.size(); j++) {
            if (condition_string[j] == ':') break;
            compare_string += condition_string[j];
        }
        condition.compare_to = std::stoi(compare_string);

        for (int j = condition_string.size() - 1; j >= 0; j--) {
            if (condition_string[j] == ':') break;
            condition.match += condition_string[j];
        }
        std::reverse(condition.match.begin(), condition.match.end());

        conditions.push_back(condition);
        line.erase(0, condition_string.size() + 1);
    }

    function.conditions = conditions;

    return function;
}

static Input parse_input(string& line) {
    line.erase(0, 1);
    line.erase(line.size() - 1, 1);

    Input input;

    while (!line.empty()) {
        string variable_string;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') break;
            variable_string += line[i];
        }

        string number;
        for (int i = 2; i < variable_string.size(); i++) number += variable_string[i];

        if (variable_string[0] == 'x') input.x = std::stoi(number);
        else if (variable_string[0] == 'm') input.m = std::stoi(number);
        else if (variable_string[0] == 'a') input.a = std::stoi(number);
        else if (variable_string[0] == 's') input.s = std::stoi(number);

        line.erase(0, variable_string.size() + 1);
    }

    return input;
}

void day19::solve(const string& input) {
    if (std::ifstream file(input); file.is_open()) {
        int result_p1 = 0;

        bool finished_parsing_functions = false;

        map<string, Function> functions;
        vector<Input> inputs;

        string line;
        while (getline(file, line)) {
            if (line.empty()) {
                finished_parsing_functions = true;
                continue;
            }

            if (!finished_parsing_functions) {
                Function function = parse_function(line);
                functions.insert(pair(function.name, function));
            } else inputs.push_back(parse_input(line));
        }

        for (int i = 0; i < inputs.size(); i++) {
            const Input input = inputs[i];

            string result = test_input(input, functions["in"]);
            while (true) {
                result = test_input(input, functions[result]);
                if (result == "A" || result == "R") break;
            }

            if (result == "A") result_p1 += input.x + input.m + input.a + input.s;
        }

        std::unordered_map<char, pair<int, int>> ranges;
        ranges['x'] = pair(1, 4000);
        ranges['m'] = pair(1, 4000);
        ranges['a'] = pair(1, 4000);
        ranges['s'] = pair(1, 4000);
        long long result_p2 = sum_count_accepted(functions, ranges, "in");

        std::cout << "Solution problem 2: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}