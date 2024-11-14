#include "../includes.h"
#include "day6.h"

void day6::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<int> times;
        vector<int> distances;

        // get times
        string current_line;
        getline(file, current_line);
        current_line += " ";

        string current_number;
        for (char c : current_line) {
            if (isdigit(c)) current_number += c;
            else {
                if (!current_number.empty()) {
                    times.push_back(std::stoi(current_number));
                    current_number.clear();
                }
            }
        }

        // get distances
        getline(file, current_line);
        current_line += " ";

        for (char c : current_line) {
            if (isdigit(c)) current_number += c;
            else {
                if (!current_number.empty()) {
                    distances.push_back(std::stoi(current_number));
                    current_number.clear();
                }
            }
        }

        // solve problem 1
        int result_p1 = 1;
        for (int i = 0; i < times.size(); i++) {
            int count_wins = 0;
            for (int t = 0; t < times[i]; t++) if (t * (times[i] - t) > distances[i]) count_wins++;
            result_p1 *= count_wins;
        }

        // solve problem 2
        string new_time;
        for (int t : times) new_time += std::to_string(t);
        long time = std::stol(new_time);

        string new_distance;
        for (int d : distances) new_distance += std::to_string(d);
        long distance = std::stol(new_distance);

        int result_p2 = 0;
        for (long i = 0; i < time; i++) {
            if (i * (time - i) > distance) result_p2++;
        }

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}
