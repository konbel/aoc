#include "../includes.h"
#include "day2.h"

static pair<string, int> parse_value(string current_value_string) {
    const size_t i = current_value_string.find(" ");
    string digits = current_value_string.substr(0, i);
    return pair<string, int>(current_value_string.substr(i + 1, current_value_string.length()), std::stoi(current_value_string.substr(0, i)));
}

static map<string, int> parse_set(string current_set_string) {
    map<string, int> current_set;

    size_t i = current_set_string.find(",");
    while (i != string::npos) {
        const string current_value_string = current_set_string.substr(0, i);
        current_set.insert(parse_value(current_value_string));
        current_set_string.erase(0, i + 2);
        i = current_set_string.find(",");
    }

    return current_set;
}

static vector<map<string, int>> parse_line(string current_line) {
    vector<map<string, int>> current_sets;

    size_t i = current_line.find(";");
    while (i != string::npos) {
        string current_set_string = current_line.substr(0, i);
        current_sets.push_back(parse_set(current_set_string + ","));
        current_line.erase(0, i + 2);
        i = current_line.find(";");
    }

    return current_sets;
}

void day2::solve(const string &input) {
    constexpr int RED_CUBES = 12;
    constexpr int GREEN_CUBES = 13;
    constexpr int BLUE_CUBES = 14;
    const string COLORS[] = { "red", "green", "blue" };

    if (std::ifstream file(input); file.is_open()) {
        int result_p1 = 0;
        int result_p2 = 0;

        int line_count = 1;
        string current_line;
        while (std::getline(file, current_line)) {
            current_line.erase(0, current_line.find(":") + 2);

            auto parsed_line = parse_line(current_line + ";");

            bool is_line_possible = true;

            int max_red = std::numeric_limits<int>::min();
            int max_green = std::numeric_limits<int>::min();
            int max_blue = std::numeric_limits<int>::min();
            for (int i = 0; i < parsed_line.size(); i++) {
                // check if number is smaller than the given number
                if (parsed_line[i]["red"] > RED_CUBES) is_line_possible = false;
                if (parsed_line[i]["green"] > GREEN_CUBES) is_line_possible = false;
                if (parsed_line[i]["blue"] > BLUE_CUBES) is_line_possible = false;

                // get the minimum number of each color
                if (parsed_line[i]["red"] != 0 && parsed_line[i]["red"] > max_red) max_red = parsed_line[i]["red"];
                if (parsed_line[i]["green"] != 0 && parsed_line[i]["green"] > max_green) max_green = parsed_line[i]["green"];
                if (parsed_line[i]["blue"] != 0 && parsed_line[i]["blue"] > max_blue) max_blue = parsed_line[i]["blue"];
            }
            if (is_line_possible) result_p1 += line_count;
            int power = max_red * max_green * max_blue;
            result_p2 += power;

            line_count++;
        }

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;
    } else std::cout << "Can't open file" << std::endl;
}