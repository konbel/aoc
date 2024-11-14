#include "../includes.h"
#include "day5.h"

static vector<long> parse_numbers(const string &line) {
    vector<long> numbers;
    string current_number;

    for (const char c : line) {
        if (isdigit(c)) {
            current_number += c;
        } else {
            if (!current_number.empty()) {
                numbers.push_back(std::stol(current_number));
                current_number = "";
            }
        }
    }

    numbers.push_back(std::stol(current_number));

    return numbers;
}

static void map_value(vector<vector<long>> maps[7], long &lowest_location, long value) {
    for (int i = 0; i < 7; i++) {
        if (value > lowest_location) return;

        const auto &map = maps[i];

        for (auto entry : map) {
            const long destination_start = entry[0];
            const long source_start = entry[1];

            if (value < source_start || value > source_start + entry[2] - 1) continue;

            const long offset_to_source_start = value - source_start;
            value = destination_start + offset_to_source_start;

            break;
        }
    }

    if (value < lowest_location) {
        lowest_location = value;
    }
}

void day5::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        string seeds_string;
        getline(file, seeds_string);

        seeds_string.erase(0, seeds_string.find(':') + 2);

        vector<long> seeds = parse_numbers(seeds_string);

        // parse maps
        vector<vector<long>> maps[7];

        int index = -1; // -1 to account for the empty line between seeds and maps
        string line;

        while (getline(file, line)) {
            if (isalpha(line[0])) continue;
            if (line.empty()) {
                index++;
                continue;
            }

            maps[index].push_back(parse_numbers(line));
        }

        // map all seeds to a location
        long lowest_location_1 = 999999999999999;
        for (auto seed : seeds) {
            map_value(maps, lowest_location_1, seed);
        }

        // select seeds from ranges
        vector<long> seeds2;

        for (int i = 0; i < seeds.size(); i += 2) {
            const long seed_range_start = seeds[i];
            const long seed_range_end = seed_range_start + seeds[i + 1] - 1;

            seeds2.push_back(seed_range_start);

            for (auto entry : maps[0]) {
                const long source_start = entry[1];
                const long source_end = source_start + entry[2] - 1;

                if (seed_range_start > source_end) continue;
                if (source_start >= seed_range_start && source_start <= seed_range_end) seeds2.push_back(source_start);
            }
        }

        // map all selected seeds to a location
        long lowest_location_2 = 999999999999999;
        for (auto seed : seeds2) {
            map_value(maps, lowest_location_2, seed);
        }

        std::cout << "Solution problem 1: " << lowest_location_1 << std::endl;
        std::cout << "Solution problem 2: " << lowest_location_2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}