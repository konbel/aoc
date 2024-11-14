#include "../includes.h"
#include "day3.h"

#include <regex>

static bool find_symbols(const vector<string>& input_list, int start_x, int start_y, int length) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= length; ++j) {
            if (start_y + i >= 0 && start_y + i < input_list.size() &&
                start_x + j >= 0 && start_x + j < input_list[start_y + i].size()) {
                if (!std::regex_match(string(1, input_list[start_y + i][start_x + j]), std::regex("[\\d\\.\\n]"))) {
                    return true;
                }
            }
        }
    }

    return false;
}

static int calculate_gear_ratio(const vector<string>& input_list, vector<vector<int>> input_numbers, int start_x, int start_y) {
    int count_adjacent_numbers = 0;
    int gear_ratio = 1;

    // loop all adjacent characters
    for (int i = start_y - 1; i <= start_y + 1; i++) {
        for (int j = start_x - 1; j <= start_x + 1; j++) {
            // check bound
            if (i >= 0 && i < input_list.size() && j >= 0 && j < input_list[i].size()) {
                // check if current char is digit
                if (isdigit(input_list[i][j])) {
                    // find it in the input numbers
                    for (int k = 0; k < input_numbers[i].size(); k += 3) {
                        const auto start = input_numbers[i][k];
                        const int end = start + input_numbers[i][k + 1];

                        // number found
                        if (j >= start && j < end) {
                            count_adjacent_numbers++;
                            gear_ratio *= input_numbers[i][k + 2];
                            j = end;
                        }
                    }
                }
            }
        }
    }

    if (count_adjacent_numbers == 2) return gear_ratio;

    return 0;
}

void day3::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<string> lines;
        vector<vector<int>> num_positions;

        // Save each line into a vector for easier access
        string current_line;
        while (std::getline(file, current_line)) lines.push_back(current_line);

        // For every number we find, we store the position and the length (to make the box around it) and the value of the number to add it if it's valid
        for (size_t index = 0; index < lines.size(); ++index) {
            vector<int> temp_vector;
            // Regex for finding all sequences of digits
            std::regex digit_regex("\\d+");
            auto digit_iterator = std::sregex_iterator(lines[index].begin(), lines[index].end(), digit_regex);
            auto digit_end = std::sregex_iterator();

            for (; digit_iterator != digit_end; ++digit_iterator) {
                auto match = *digit_iterator;
                temp_vector.push_back(match.position());
                temp_vector.push_back(match.length());
                temp_vector.push_back(std::stoi(match.str()));
            }
            num_positions.push_back(temp_vector);
        }

        int valid_nums = 0;
        int gear_ratios = 0;

        // Adds all the valid numbers found
        for (size_t index = 0; index < num_positions.size(); ++index) {
            for (size_t i = 0; i < num_positions[index].size(); i += 3) {
                if (find_symbols(lines, num_positions[index][i], static_cast<int>(index), num_positions[index][i + 1])) {
                    valid_nums += num_positions[index][i + 2];
                }
            }
        }

        // Calculates gear ratios for each gear
        for (int index = 0; index < lines.size(); ++index) {
            for (int i = 0; i < lines[index].size(); ++i) {
                if (lines[index][i] == '*') {
                    gear_ratios += calculate_gear_ratio(lines, num_positions, i, index);
                }
            }
        }

        std::cout << "Solution problem 1: " << valid_nums << std::endl;
        std::cout << "Solution problem 2: " << gear_ratios << std::endl;

        file.close();
    } else {
        std::cout << "Can't open file" << std::endl;
    }
}