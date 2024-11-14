#include "../includes.h"
#include "day16.h"

struct Ray {
    vector<int> position;
    vector<int> direction;
};

static int shoot_ray(const vector<string>& grid, const vector<int>& start_position, const vector<int>& direction) {
    map<vector<int>, bool> energized_fields;

    map<vector<int>, int> rays_seen;
    vector<Ray> rays;

    Ray start_ray = Ray();
    start_ray.position = start_position;
    start_ray.direction = direction;
    rays.push_back(start_ray);
    rays_seen[start_ray.position] = true;

    while (!rays.empty()) {
        while (true) {
            rays[0].position[0] += rays[0].direction[0];
            rays[0].position[1] += rays[0].direction[1];
            if (rays[0].position[0] < 0 || rays[0].position[0] >= grid[0].size() || rays[0].position[1] < 0 || rays[0].position[1] >= grid.size()) break;
            energized_fields[rays[0].position] = true;

            const char current_object = grid[rays[0].position[1]][rays[0].position[0]];
            if (current_object == '.') continue;

            if (current_object == '/') {
                if (rays[0].direction[0] == 1) rays[0].direction = {0, -1}; // coming from left
                else if (rays[0].direction[0] == -1) rays[0].direction = {0, 1}; // coming from right
                else if (rays[0].direction[1] == 1) rays[0].direction = {-1, 0}; // coming from top
                else if (rays[0].direction[1] == -1) rays[0].direction = {1, 0}; // coming from bottom
            } else if (current_object == '\\') {
                if (rays[0].direction[0] == 1) rays[0].direction = {0, 1}; // coming from left
                else if (rays[0].direction[0] == -1) rays[0].direction = {0, -1}; // coming from right
                else if (rays[0].direction[1] == 1) rays[0].direction = {1, 0}; // coming from top
                else if (rays[0].direction[1] == -1) rays[0].direction = {-1, 0}; // coming from bottom
            }

            if (current_object == '-') {
                if (rays[0].direction[1] == 0) continue;

                Ray left_ray = Ray();
                left_ray.position = rays[0].position;
                left_ray.direction = {-1, 0};

                if (rays_seen[left_ray.position] < 2) {
                    rays.push_back(left_ray);
                    rays_seen[left_ray.position]++;
                }

                Ray right_ray = Ray();
                right_ray.position = rays[0].position;
                right_ray.direction = {1, 0};

                if (rays_seen[right_ray.position] < 2) {
                    rays.push_back(right_ray);
                    rays_seen[right_ray.position]++;
                }

                break;
            }

            if (current_object == '|') {
                if (rays[0].direction[0] == 0) continue;

                Ray top_ray = Ray();
                top_ray.position = rays[0].position;
                top_ray.direction = {0, -1};

                if (rays_seen[top_ray.position] < 2) {
                    rays.push_back(top_ray);
                    rays_seen[top_ray.position]++;
                }

                Ray down_ray = Ray();
                down_ray.position = rays[0].position;
                down_ray.direction = {0, 1};

                if (rays_seen[down_ray.position] < 2) {
                    rays.push_back(down_ray);
                    rays_seen[down_ray.position]++;
                }

                break;
            }
        }
        rays.erase(rays.begin(), rays.begin() + 1);
    }

    return energized_fields.size();
}

void day16::solve(const string& input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<string> grid;

        string line;
        while (getline(file, line)) grid.push_back(line);

        const int result_p1 = shoot_ray(grid, {-1, 0}, {1, 0});

        vector<int> results;

        for (int i = 0; i < grid[0].size(); i++) results.push_back(shoot_ray(grid, {i, -1}, {0, 1})); // top row
        for (int i = 0; i < grid[0].size(); i++) results.push_back(shoot_ray(grid, {i, static_cast<int>(grid.size())}, {0, -1})); // bottom row

        for (int i = 0; i < grid.size(); i++) results.push_back(shoot_ray(grid, {-1, i}, {1, 0})); // left row
        for (int i = 0; i < grid.size(); i++) results.push_back(shoot_ray(grid, {static_cast<int>(grid[0].size()), i}, {-1, 0})); // left row

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << std::ranges::max(results) << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}
