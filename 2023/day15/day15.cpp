#include "../includes.h"
#include "day15.h"

static int hash(const string& to_hash) {
    int hash = 0;
    for (char c : to_hash) {
        hash += static_cast<int>(c);
        hash *= 17;
        hash = hash % 256;
    }
    return hash;
}


void day15::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        string line;
        getline(file, line);
        line += ',';

        int result_p1 = 0;

        map<int, vector<pair<string, int>>> boxes;

        string to_hash = "";
        int hash_result = 0;
        for (char c : line) {
            if (c == ',') {
                // hash entire string
                hash_result = hash(to_hash);
                result_p1 += hash_result;

                // hash label
                int pos_minus = to_hash.find('-');
                int pos_equal = to_hash.find('=');
                string label = pos_minus == string::npos ? to_hash.substr(0, pos_equal) : to_hash.substr(0, pos_minus);
                int hash_label = hash(label);

                auto& box = boxes[hash_label];
                if (pos_minus != string::npos) {
                    for (int i = 0; i < box.size(); i++) {
                        if (box[i].first == label) {
                            box.erase(box.begin() + i, box.begin() + i + 1);
                            break;
                        }
                    }
                } else {
                    int focal_length = to_hash[to_hash.size() - 1] - '0';

                    bool found = false;
                    for (int i = 0; i < box.size(); i++) {
                        if (box[i].first == label) {
                            box[i].second = focal_length;
                            found = true;
                            break;
                        }
                    }

                    if (!found) box.push_back(pair(label, focal_length));
                }

                to_hash = "";
                continue;
            }
            to_hash += c;
        }

        int result_p2 = 0;
        for (int i = 0; i < boxes.size(); i++) {
            for (int j = 0; j < boxes[i].size(); j++) {
                int focusing_power = (i + 1) * (j + 1) * boxes[i][j].second;
                result_p2 += focusing_power;
            }
        }

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

    } else std::cout << "Can't open file" << std::endl;
}