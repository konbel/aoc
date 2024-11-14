#include "../includes.h"
#include "day21.h"

#include <cassert>

static vector<int> add_vector(const vector<int>& v1, const vector<int>& v2) {
    return {v1[0] + v2[0], v1[1] + v2[1]};
}

static bool check_bounds(const int& max_x, const int& max_y, const vector<int>& position) {
    if (position[0] >= 0 && position[0] < max_x && position[1] >= 0 && position[1] < max_y) return true;
    return false;
}

static int take_steps(vector<string>& garden, const vector<int>& start_position, const int& steps_to_take) {
    const vector<vector<int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    vector<vector<int>> move_queue;
    vector<vector<int>> new_moves;
    move_queue.push_back(start_position);

    for (int i = 0; i < steps_to_take; i++) {
        while (!move_queue.empty()) {
            vector position = move_queue[0];

            for (vector<int> direction : directions) {
                vector<int> new_position = add_vector(position, direction);
                if (!check_bounds(garden[0].size(), garden.size(), new_position)) continue;

                if (garden[new_position[1]][new_position[0]] == '.') {
                    new_moves.push_back(new_position);
                    garden[new_position[1]][new_position[0]] = 'O';
                }
            }

            garden[position[1]][position[0]] = '.';
            move_queue.erase(move_queue.begin(), move_queue.begin() + 1);
        }

        move_queue = new_moves;
        new_moves.clear();
    }

    int count_reached = 0;
    for (const string& l : garden) {
        for (const char& c : l) {
            if (c == 'O') count_reached++;
        }
    }

    return count_reached;
}
static long long calc_steps(vector<string>& garden, const vector<int>& start_position, const int& steps_to_take) {
    const vector<vector<int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    assert(garden.size() == garden[0].size());
    const int size = garden.size();

    assert(steps_to_take % size == size / 2);

    const long grid_width = steps_to_take / size - 1;
    const long odd = (grid_width / 2 * 2 + 1) * (grid_width / 2 * 2 + 1);
    const long even = ((grid_width + 1) / 2 * 2) * ((grid_width + 1) / 2 * 2);

    vector<string> garden_copy = garden;
    const int odd_points = take_steps(garden_copy, start_position, size * 2 + 1) * 2;

    garden_copy = garden;
    const int event_points = take_steps(garden_copy, start_position, size * 2) * 2;

    garden_copy = garden;
    const int corner_t = take_steps(garden_copy, {start_position[0], size - 1}, size - 1);

    garden_copy = garden;
    const int corner_r = take_steps(garden_copy, {0, start_position[1]}, size - 1);

    garden_copy = garden;
    const int corner_b = take_steps(garden_copy, {start_position[0], 0}, size - 1);

    garden_copy = garden;
    const int corner_l = take_steps(garden_copy, {size - 1, start_position[1]}, size - 1);


    garden_copy = garden;
    const int small_tr = take_steps(garden_copy, {0, size - 1}, size / 2 - 1);

    garden_copy = garden;
    const int small_tl = take_steps(garden_copy, {size - 1, size - 1 }, size / 2 - 1);

    garden_copy = garden;
    const int small_br = take_steps(garden_copy, {0, 0}, size / 2 - 1);

    garden_copy = garden;
    const int small_bl = take_steps(garden_copy, {size - 1, 0}, size / 2 - 1);


    garden_copy = garden;
    const int large_tr = take_steps(garden_copy, {0, size - 1}, size * 3 / 2 - 1);

    garden_copy = garden;
    const int large_tl = take_steps(garden_copy, {size - 1, size - 1 }, size * 3 / 2 - 1);

    garden_copy = garden;
    const int large_br = take_steps(garden_copy, {0, 0}, size * 3 / 2 - 1);

    garden_copy = garden;
    const int large_bl = take_steps(garden_copy, {size - 1, 0}, size * 3 / 2 - 1);

    const long long result = odd * odd_points +
         even * event_points +
         corner_t + corner_r + corner_b + corner_l +
         (grid_width + 1) * (small_tr + small_tl + small_br + small_bl) +
         grid_width * (large_tl + large_tr + large_bl + large_br);

    return result;
}

void day21::solve(const string& input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<string> garden;
        vector<int> start_position;

        string line;
        while (getline(file, line)) {
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == 'S') {
                    start_position = {i, static_cast<int>(garden.size())};
                    line[i] = '.';
                    break;
                }
            }
            garden.push_back(line);
        }

        const int result_p1 = take_steps(garden, start_position, 64);
        const long long result_p2 = calc_steps(garden, start_position, 26501365); // 594115391548176

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

    } else std::cout << "Can't open file" << std::endl;
}