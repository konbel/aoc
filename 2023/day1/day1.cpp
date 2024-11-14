#include "../includes.h"
#include "day1.h"

void day1::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        int result_p1 = 0;
        int result_p2 = 0;

        const string number_strings[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

        // read file line by line
        string current_line;
        while (std::getline(file, current_line)) {
            map<int, string> current_numbers_p1;
            map<int, string> current_numbers_p2;

            // scan line for digits before replace
            for (int i = 0; i < current_line.length(); i++) {
                if (isdigit(current_line[i])) current_numbers_p1.insert(pair<int, string>(i, string {current_line[i]}));
            }

            // replace number strings with digits
            for (int i = 0; i < 9; i++) {
                size_t index = current_line.find(number_strings[i], 0);
                while (index != string::npos) {
                    // replace number string with digit
                    current_line[index + 1] = i + 1 + '0';
                    for (int j = index + 2; j < number_strings[i].length() - 1; j++) current_line[j] = ' ';

                    // try to find next sub string
                    index = current_line.find(number_strings[i], index + 1);
                }
            }

            // scan line for digits after replace
            for (int i = 0; i < current_line.length(); i++) {
                if (isdigit(current_line[i])) current_numbers_p2.insert(pair<int, string>(i, string {current_line[i]}));
            }

            // join first and last digit string and add to total count
            result_p1 += std::stoi(current_numbers_p1.begin()->second + (--current_numbers_p1.end())->second);
            result_p2 += std::stoi(current_numbers_p2.begin()->second + (--current_numbers_p2.end())->second);
        }

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}