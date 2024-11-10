//
// Created by Konstantin Kilbel on 25.12.23.
//

#include "../includes.h"
#include "day25.h"

void day25::solve(const string& input) {
    std::ifstream file(input);
    if (file.is_open()) {
        map<string, vector<string>> modules;

        string line;
        while (getline(file, line)) {
            string name = line.substr(0, 3);
            vector<string> connected;

            string cur;
            for (int i = 5; i < line.size(); i++) {
                if (line[i] == ' ') {
                    connected.push_back(cur);
                    cur.clear();
                    i++;
                }
                cur += line[i];
            }

            modules[name] = connected;
        }

        file.close();
    } else cout << "Can't open file" << endl;
}