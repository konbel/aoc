#include "../includes.h"
#include "day10.h"

map<char, vector<int>> pipes = {
    {'L', {1, -1}},
    {'J', {-1, -1}},
    {'7', {-1, 1}},
    {'F', {1, 1}},
    {'|', {}},
    {'-', {}},
};

vector<int> find_and_replace_start(vector<vector<char>> &maze) {
    vector start = { -1, -1 };

    for (int r = 0; r < maze.size(); r++) {
        for (int c = 0; c < maze[r].size(); c++) {
            if (maze[r][c] == 'S') {
                start = { c, r };
                break;
            }
        }

        if (start != vector{ -1, -1 }) break;
    }

    if (start[1] > 0) { // |, J, L
        char above = maze[start[1] - 1][start[0]];

        if (above == '|' || above == '7' || above == 'F') { // |, J, L
            if (start[1] < maze.size() - 1) {
                char below = maze[start[1] + 1][start[0]];

                if (below == '|' || below == 'J' || below == 'L') { // |
                    maze[start[1]][start[0]] = '|';
                    return start;
                }
            }

            if (start[0] > 0) { // J
                char left = maze[start[1]][start[0] - 1];

                if (left == '-' || left == 'L' || left == 'F') {
                    maze[start[1]][start[0]] = 'J';
                    return start;
                }
            }

            if (start[0] < maze.size() - 1) { // L
                char right = maze[start[1]][start[0] + 1];

                if (right == '-' || right == '7' || right == 'J') {
                    maze[start[1]][start[0]] = 'L';
                    return start;
                }
            }
        }
    }

    if (start[1] < maze.size() - 1) { // 7, F
        char below = maze[start[1] + 1][start[0]];

        if (below == '|' || below == 'J' || below == 'L') { // 7, F
            if (start[0] > 0) { // 7
                char left = maze[start[1]][start[0] - 1];

                if (left == '-' || left == 'L' || left == 'F') {
                    maze[start[1]][start[0]] = '7';
                    return start;
                }
            }

            if (start[0] < maze[0].size() - 1) { // F
                char right = maze[start[1]][start[0] + 1];

                if (right == '-' || right == 'J' || right == '7') {
                    maze[start[1]][start[0]] = 'F';
                    return start;
                }
            }
        }
    }

    if (start[0] > 0 && start[0] < maze.size() - 1) {
        char left = maze[start[1]][start[0] - 1];
        char right = maze[start[1]][start[0] + 1];

        if ((left == '-' || left == 'L' || left == 'F')
            && right == '-' || right == 'J' || right == '7') {
            maze[start[1]][start[0]] = '-';
            return start;
        }
    }

    return start; // this should never be reached if the input is valid
}

void flood_fill(vector<vector<char>> &maze, const std::set<vector<int>> &loop_tiles, const vector<int> &start) {
    std::queue<vector<int>> tiles_to_fill;
    std::set<vector<int>> seen;
    tiles_to_fill.push(start);
    seen.insert(start);

    while (!tiles_to_fill.empty()) {
        vector<int> current_tile = tiles_to_fill.front();
        tiles_to_fill.pop();

        if (loop_tiles.contains(current_tile)) continue;

        maze[current_tile[1]][current_tile[0]] = ' ';

        // left
        if (current_tile[0] > 0) {
            vector<int> tile_left = current_tile;
            tile_left[0]--;

            if (!seen.contains(tile_left)) {
                seen.insert(tile_left);
                tiles_to_fill.push(tile_left);
            }
        }

        // right
        if (current_tile[0] < maze[0].size() - 1) {
            vector<int> tile_right = current_tile;
            tile_right[0]++;

            if (!seen.contains(tile_right)) {
                seen.insert(tile_right);
                tiles_to_fill.push(tile_right);
            }
        }

        // above
        if (current_tile[1] > 0) {
            vector<int> tile_above = current_tile;
            tile_above[1]--;

            if (!seen.contains(tile_above)) {
                seen.insert(tile_above);
                tiles_to_fill.push(tile_above);
            }
        }

        // below
        if (current_tile[1] < maze.size() - 1) {
            vector<int> tile_above = current_tile;
            tile_above[1]++;

            if (!seen.contains(tile_above)) {
                seen.insert(tile_above);
                tiles_to_fill.push(tile_above);
            }
        }
    }
}

std::set<vector<int>> get_loop_tiles(const vector<vector<char>> &maze, const vector<int> &start_position) {
    vector<int> current_position = start_position;
    vector<int> last_position = start_position;

    switch (maze[start_position[1]][start_position[0]]) {
        case '|':
        case 'J':
        case 'L':
            current_position[1]--;
        break;

        case '-':
            current_position[0]++;
        break;

        case '7':
        case 'F':
            current_position[1]++;
        break;

        default:
            break;
    }

    std::set<vector<int>> loop_tiles = { start_position, current_position };

    while (current_position != start_position) {
        char current_pipe = maze[current_position[1]][current_position[0]];

        if (pipes.contains(current_pipe)) {
            auto old_position = current_position;

            if (current_pipe == '|') { // vertical movement
                if (last_position[1] < current_position[1]) current_position[1]++;
                else current_position[1]--;
            } else if (current_pipe == '-') { // horizontal movement
                if (last_position[0] < current_position[0]) current_position[0]++;
                else current_position[0]--;
            } else { // corner movement
                if (last_position[0] != current_position[0]) current_position[1] += pipes[current_pipe][1];
                else current_position[0] += pipes[current_pipe][0];
            }

            loop_tiles.insert(current_position);
            last_position = old_position;
        }
    }

    return loop_tiles;
}

void day10::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<vector<char>> maze;

        // read input file
        string current_line;
        while (getline(file, current_line)) {
            maze.emplace_back(current_line.begin(), current_line.end());
        }

        vector<int> start = find_and_replace_start(maze);
        std::set loop_tiles = get_loop_tiles(maze, start);

        // remove all pipes not belonging to the loop
        for (int r = 0; r < maze.size(); r++) {
            for (int c = 0; c < maze[r].size(); c++) {
                vector current_position = { c, r };
                if (!loop_tiles.contains(current_position)) {
                    maze[r][c] = '.';
                }
            }
        }

        // double maze resolution
        int double_maze_width = maze[0].size() * 2;
        int double_maze_height = maze.size() * 2;

        vector double_maze(double_maze_height, vector(double_maze_width, '.'));

        for (int r = 0; r < maze.size(); r++) {
            for (int c = 0; c < maze[r].size(); c++) {
                int double_r = r * 2;
                int double_c = c * 2;

                double_maze[double_r][double_c] = maze[r][c];

                if (double_maze[double_r][double_c] == '|' || double_maze[double_r][double_c] == 'F' || double_maze[double_r][double_c] == '7') {
                    double_maze[double_r + 1][double_c] = '|';
                }

                if (double_maze[double_r][double_c] == '-' || double_maze[double_r][double_c] == 'L' || double_maze[double_r][double_c] == 'F') {
                    double_maze[double_r][double_c + 1] = '-';
                }
            }
        }

        vector double_start = { start[0] * 2, start[1] * 2 };
        std::set<vector<int>> double_loop_tiles = get_loop_tiles(double_maze, double_start);

        /*
         * The test input doesn't always contain an empty first and last row and column.
         * So the flood fill has to start from corner to account for that although every input should have empty rows / columns.
         * This can still fail if there are parts in the middle of each edge that cant be reached from the corner.
         * But I think that is impossible for the input, so I will ignore this case.
         * If this happens to be a problem to you just start flood fill from every edge position and you should be fine.
         */
        flood_fill(double_maze, double_loop_tiles, vector{ 0, 0 });
        flood_fill(double_maze, double_loop_tiles, vector{ 0, double_maze_height - 1 });
        flood_fill(double_maze, double_loop_tiles, vector{ double_maze_width - 1, 0 });
        flood_fill(double_maze, double_loop_tiles, vector{ double_maze_width - 1, double_maze_height - 1 });

        // count remaining dots
        int count = 0;

        for (int r = 0; r < double_maze_height; r++) {
            if (r % 2 != 0) continue;

            for (int c = 0; c < double_maze_width; c++) {
                if (c % 2 != 0) continue;

                if (double_maze[r][c] == '.') {
                    double_maze[r][c] = 'x';
                    count++;
                }
            }
        }

        // for (int r = 0; r < double_maze_height; r++) {
        //     for (int c = 0; c < double_maze_width; c++) {
        //         printf("%c", double_maze[r][c]);
        //     }
        //     printf("\n");
        // }

        std::cout << "Solution problem 1: " << (loop_tiles.size() + 1) / 2 << std::endl;
        std::cout << "Solution problem 2: " << count << std::endl;

    } else std::cout << "Can't open file" << std::endl;
}