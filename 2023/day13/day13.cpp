//
// Created by Konstantin Kilbel on 13.12.23.
//

#include "../includes.h"
#include "day13.h"

int day13::checkMirrorP1(vector<string> rows) {
    for (int i = 1; i < rows.size(); i++) {
        vector<string> above(rows.begin(), rows.begin() + i);
        std::reverse(above.begin(), above.end());

        vector<string> below(rows.begin() + i, rows.end());

        above.resize(std::min(above.size(), below.size()));
        below.resize(std::min(above.size(), below.size()));

        if (above == below) return i;
    }

    return 0;
}

int day13::checkMirrorP2(vector<string> rows) {
    for (int i = 1; i < rows.size(); i++) {
        vector<string> above(rows.begin(), rows.begin() + i);
        std::reverse(above.begin(), above.end());

        vector<string> below(rows.begin() + i, rows.end());

        above.resize(std::min(above.size(), below.size()));
        below.resize(std::min(above.size(), below.size()));

        int mismatches = 0;
        for (int i = 0; i < above.size(); i++) {
            for (int j = 0; j < above[i].size(); j++) {
                if (above[i][j] != below[i][j]) mismatches++;
            }
        }

        if (mismatches == 1) return i;
    }

    return 0;
}


void day13::solve(string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        int resultP1 = 0;
        int resultP2 = 0;

        vector<string> rows;
        vector<string> columns;

        string line;
        while (getline(file, line)) {
            if (line.empty()) {
                // check rows
                resultP1 += checkMirrorP1(rows) * 100;
                resultP1 += checkMirrorP1(columns);

                resultP2 += checkMirrorP2(rows) * 100;
                resultP2 += checkMirrorP2(columns);

                columns.clear();
                rows.clear();
                continue;
            }

            // insert into rows
            rows.push_back(line);

            // insert into columns
            for (int c = 0; c < line.size(); c++) {
                if (c >= columns.size()) columns.push_back(string{line[c]});
                else columns[c].push_back(line[c]);
            }
        }

        resultP1 += checkMirrorP1(rows) * 100;
        resultP1 += checkMirrorP1(columns);

        resultP2 += checkMirrorP2(rows) * 100;
        resultP2 += checkMirrorP2(columns);

        cout << "solution problem 1: " << resultP1 << endl;
        cout << "solution problem 1: " << resultP2 << endl;

        file.close();
    } else cout << "Can't open file" << endl;
}