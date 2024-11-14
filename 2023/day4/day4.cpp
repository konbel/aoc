#include "../includes.h"
#include "day4.h"

void day4::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        int result_p1, result_p2;
        vector<int> matches;

        // read file line by line
        string current_line;
        int lines;
        while (getline(file, current_line)) {
            lines++;

            // remove prefix
            current_line.erase(0, current_line.find(":") + 2);

            // split string by seperator
            int seperator = current_line.find("|");
            string winning_numbers_string = current_line.substr(0, seperator);
            current_line.erase(0, seperator + 2);

            // remove leading space
            if (current_line[0] == ' ') current_line.erase(0, 1);
            if (winning_numbers_string[0] == ' ') winning_numbers_string.erase(0, 1);

            // add trailing space
            current_line += " ";
            winning_numbers_string += " ";

            // parse winning numbers
            vector<int> winning_numbers;
            string n;
            for (int i = 0; i < current_line.size(); i++) {
                if (isdigit(winning_numbers_string[i])) n += winning_numbers_string[i];
                else {
                    if (!n.empty()) {
                        winning_numbers.push_back(std::stoi(n));
                        n = string();
                    }
                }
            }

            // parse current numbers
            vector<int> current_numbers;
            for (int i = 0; i < current_line.size(); i++) {
                if (isdigit(current_line[i])) n += current_line[i];
                else {
                    if (!n.empty()) {
                        current_numbers.push_back(std::stoi(n));
                        n = string();
                    }
                }
            }

            // compare current and winning numbers
            int current_result = 0;
            int current_matches = 0;
            for (int c : current_numbers) {
                for (int w : winning_numbers) {
                    if (c == w) {
                        current_matches++;
                        if (current_result == 0) current_result = 1;
                        else current_result *= 2;
                    }
                }
            }

            matches.push_back(current_matches);

            result_p1 += current_result;
        }

        // calculate number of copies
        vector<int> repeat_line_count(lines);
        for (int& i : repeat_line_count) i = 1;

        for (int i = 0; i < repeat_line_count.size(); i++) {
            for (int j = 0; j < repeat_line_count[i]; j++) {
                for (int k = 1; k <= matches[i]; k++) {
                    repeat_line_count[i + k]++;
                }
            }
        }

        // sum number of copies
        for (int i : repeat_line_count) result_p2 += i;

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 <<  std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}