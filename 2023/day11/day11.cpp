//
// Created by Konstantin Kilbel on 11.12.23.
//

#include "../includes.h"
#include "day11.h"

void day11::solve(string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        vector<string> lines;

        // read file
        string currentLine;
        while (getline(file, currentLine)) lines.push_back(currentLine);

        vector<string> expanded = lines;

        vector<int> lineBreakRow;
        vector<int> lineBreakColumn;

        // expand
        std::vector<int> galaxiesPerColumn(currentLine.size());
        for (int i = 0; i < lines.size(); i++) {
            int galaxiesFound = 0;
            for (int j = 0; j < lines[i].size(); j++) {
                if (lines[i][j] == '#') {
                    galaxiesFound++;
                    galaxiesPerColumn[j]++;
                }
            }

            if (galaxiesFound == 0) lineBreakRow.push_back(i);
        }

        for (int i = 0; i < galaxiesPerColumn.size(); i++) {
            if (galaxiesPerColumn[i] == 0) lineBreakColumn.push_back(i);
        }

        // get position
        vector<int> galaxiesP1;
        vector<int> galaxiesP2;
        for (int i = 0; i < lines.size(); i++) {
            for (int j = 0; j < lines[i].size(); j++) {
                if (lines[i][j] == '#') {
                    int actualRowP1 = i;
                    int actualColumP1 = j;

                    int actualRowP2 = i;
                    int actualColumP2 = j;

                    for (int k = 0; k < lineBreakRow.size(); k++) {
                        if (i > lineBreakRow[k]) {
                            actualRowP1 += 1;
                            actualRowP2 += 999999;
                        }
                    }

                    for (int k = 0; k < lineBreakColumn.size(); k++) {
                        if (j > lineBreakColumn[k]) {
                            actualColumP1 += 1;
                            actualColumP2 += 999999;
                        }
                    }

                    galaxiesP1.push_back(actualColumP1);
                    galaxiesP1.push_back(actualRowP1);

                    galaxiesP2.push_back(actualColumP2);
                    galaxiesP2.push_back(actualRowP2);
                }
            }
        }

        long resultP1;
        long resultP2;
        for (int i = 0; i < galaxiesP1.size(); i += 2) {
            for (int j = i + 2; j < galaxiesP1.size(); j += 2) {
                resultP1 += abs(galaxiesP1[j] - galaxiesP1[i]) + abs(galaxiesP1[j + 1] - galaxiesP1[i + 1]);
                resultP2 += abs(galaxiesP2[j] - galaxiesP2[i]) + abs(galaxiesP2[j + 1] - galaxiesP2[i + 1]);
            }
        }

        cout << "Solution problem 1: " << resultP1 << endl;
        cout << "Solution problem 2: " << resultP2 << endl;

        file.close();
    } else cout << "Can't open file" << endl;
}