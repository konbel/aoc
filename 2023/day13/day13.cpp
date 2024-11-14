#include "../includes.h"
#include "day13.h"

static int check_mirror_p1(vector<string> rows) {
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

static int check_mirror_p2(vector<string> rows) {
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


void day13::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        int result_p1 = 0;
        int result_p2 = 0;

        vector<string> rows;
        vector<string> columns;

        string line;
        while (getline(file, line)) {
            if (line.empty()) {
                // check rows
                result_p1 += check_mirror_p1(rows) * 100;
                result_p1 += check_mirror_p1(columns);

                result_p2 += check_mirror_p2(rows) * 100;
                result_p2 += check_mirror_p2(columns);

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

        result_p1 += check_mirror_p1(rows) * 100;
        result_p1 += check_mirror_p1(columns);

        result_p2 += check_mirror_p2(rows) * 100;
        result_p2 += check_mirror_p2(columns);

        std::cout << "solution problem 1: " << result_p1 << std::endl;
        std::cout << "solution problem 1: " << result_p2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}