//
// Created by Konstantin Kilbel on 15.12.23.
//

#include "../includes.h"
#include "day15.h"

int day15::hash(const string& toHash) {
    int hash = 0;
    for (char c : toHash) {
        hash += static_cast<int>(c);
        hash *= 17;
        hash = hash % 256;
    }
    return hash;
}


void day15::solve(string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        string line;
        getline(file, line);
        line += ',';

        int resultP1 = 0;

        std::map<int, vector<std::pair<string, int>>> boxes;

        string toHash = "";
        int hashResult = 0;
        for (char c : line) {
            if (c == ',') {
                // hash entire string
                hashResult = hash(toHash);
                resultP1 += hashResult;

                // hash label
                int posMinus = toHash.find('-');
                int posEqual = toHash.find('=');
                string label = posMinus == string::npos ? toHash.substr(0, posEqual) : toHash.substr(0, posMinus);
                int hashLabel = hash(label);

                auto& box = boxes[hashLabel];
                if (posMinus != string::npos) {
                    for (int i = 0; i < box.size(); i++) {
                        if (box[i].first == label) {
                            box.erase(box.begin() + i, box.begin() + i + 1);
                            break;
                        }
                    }
                } else {
                    int focalLength = toHash[toHash.size() - 1] - '0';

                    bool found = false;
                    for (int i = 0; i < box.size(); i++) {
                        if (box[i].first == label) {
                            box[i].second = focalLength;
                            found = true;
                            break;
                        }
                    }

                    if (!found) box.push_back(std::pair(label, focalLength));
                }

                toHash = "";
                continue;
            }
            toHash += c;
        }

        int resultP2 = 0;
        for (int i = 0; i < boxes.size(); i++) {
            for (int j = 0; j < boxes[i].size(); j++) {
                int focusingPower = (i + 1) * (j + 1) * boxes[i][j].second;
                resultP2 += focusingPower;
            }
        }

        cout << "Solution problem 1: " << resultP1 << endl;
        cout << "Solution problem 2: " << resultP2 << endl;

    } else cout << "Can't open file" << endl;
}