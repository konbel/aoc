//
// Created by Konstantin Kilbel on 19.12.23.
//

#ifndef DAY19_H
#define DAY19_H

namespace day19 {
    struct Condition {
        int variable;
        char operation;
        int compareTo;
        string match;
    };

    struct Function {
        string name;
        vector<Condition> conditions;
        string exitCase;
    };

    struct Input {
        int x, m, a, s;
    };

    long sumCountAccepted(map<string, Function>& functions, std::unordered_map<char, std::pair<int, int>>& ranges, const std::string& name);
    string testInput(const Input& input, const Function& function);
    Function parseFunction(string& line);
    Input parseInput(string& line);
    void solve(const string& input);
}

#endif //DAY19_H