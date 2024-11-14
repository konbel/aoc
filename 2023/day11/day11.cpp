#include "../includes.h"
#include "day11.h"

void day11::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<string> lines;

        // read file
        string current_line;
        while (getline(file, current_line)) lines.push_back(current_line);

        vector<string> expanded = lines;

        vector<int> line_break_row;
        vector<int> line_break_column;

        // expand
        vector<int> galaxies_per_column(current_line.size());
        for (int i = 0; i < lines.size(); i++) {
            int galaxies_found = 0;
            for (int j = 0; j < lines[i].size(); j++) {
                if (lines[i][j] == '#') {
                    galaxies_found++;
                    galaxies_per_column[j]++;
                }
            }

            if (galaxies_found == 0) line_break_row.push_back(i);
        }

        for (int i = 0; i < galaxies_per_column.size(); i++) {
            if (galaxies_per_column[i] == 0) line_break_column.push_back(i);
        }

        // get position
        vector<int> galaxies_p1;
        vector<int> galaxies_p2;
        for (int i = 0; i < lines.size(); i++) {
            for (int j = 0; j < lines[i].size(); j++) {
                if (lines[i][j] == '#') {
                    int actual_row_p1 = i;
                    int actual_column_p1 = j;

                    int actual_row_p2 = i;
                    int actual_column_p2 = j;

                    for (int k = 0; k < line_break_row.size(); k++) {
                        if (i > line_break_row[k]) {
                            actual_row_p1 += 1;
                            actual_row_p2 += 999999;
                        }
                    }

                    for (int k = 0; k < line_break_column.size(); k++) {
                        if (j > line_break_column[k]) {
                            actual_column_p1 += 1;
                            actual_column_p2 += 999999;
                        }
                    }

                    galaxies_p1.push_back(actual_column_p1);
                    galaxies_p1.push_back(actual_row_p1);

                    galaxies_p2.push_back(actual_column_p2);
                    galaxies_p2.push_back(actual_row_p2);
                }
            }
        }

        long result_p1;
        long result_p2;
        for (int i = 0; i < galaxies_p1.size(); i += 2) {
            for (int j = i + 2; j < galaxies_p1.size(); j += 2) {
                result_p1 += abs(galaxies_p1[j] - galaxies_p1[i]) + abs(galaxies_p1[j + 1] - galaxies_p1[i + 1]);
                result_p2 += abs(galaxies_p2[j] - galaxies_p2[i]) + abs(galaxies_p2[j + 1] - galaxies_p2[i + 1]);
            }
        }

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}