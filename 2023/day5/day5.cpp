#include "../includes.h"
#include "day5.h"

std::vector<long> parse_numbers(const std::string &line) {
    std::vector<long> numbers;
    std::string currentNumber;

    for (const char c : line) {
        if (isdigit(c)) {
            currentNumber += c;
        } else {
            if (!currentNumber.empty()) {
                numbers.push_back(std::stol(currentNumber));
                currentNumber = "";
            }
        }
    }

    numbers.push_back(std::stol(currentNumber));

    return numbers;
}

void map_value(std::vector<std::vector<long>> maps[7], long &lowest_location, long value) {
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

void day5::solve(const std::string &input) {
    if (std::ifstream file(input); file.is_open()) {
        std::string seedsString;
        getline(file, seedsString);

        seedsString.erase(0, seedsString.find(':') + 2);

        std::vector<long> seeds = parse_numbers(seedsString);

        // parse maps
        std::vector<std::vector<long>> maps[7];

        int index = -1; // -1 to account for the empty line between seeds and maps
        std::string line;

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
        std::vector<long> seeds2;

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