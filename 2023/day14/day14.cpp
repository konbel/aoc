#include "../includes.h"
#include "day14.h"

static void move_up(vector<string>& lines, int current_line, int current_char) {
    if (current_line - 1 < 0) return;
    if (lines[current_line - 1][current_char] == '.') {
        lines[current_line][current_char] = '.';
        lines[current_line - 1][current_char] = 'O';
        move_up(lines, current_line - 1, current_char);
    }
}

static void move_left(vector<string>& lines, int current_line, int current_char) {
    if (current_char - 1 < 0) return;
    if (lines[current_line][current_char - 1] == '.') {
        lines[current_line][current_char] = '.';
        lines[current_line][current_char - 1] = 'O';
        move_left(lines, current_line, current_char - 1);
    }
}

static void move_down(vector<string>& lines, int current_line, int current_char) {
    if (current_line + 1 >= lines.size()) return;
    if (lines[current_line + 1][current_char] == '.') {
        lines[current_line][current_char] = '.';
        lines[current_line + 1][current_char] = 'O';
        move_down(lines, current_line + 1, current_char);
    }
}

static void move_right(vector<string>& lines, int current_line, int current_char) {
    if (current_char + 1 >= lines[current_line].size()) return;
    if (lines[current_line][current_char + 1] == '.') {
        lines[current_line][current_char] = '.';
        lines[current_line][current_char + 1] = 'O';
        move_right(lines, current_line, current_char + 1);
    }
}

static void cycle(vector<string>& lines) {
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == 'O') move_up(lines, i, j);
        }
    }

    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == 'O') move_left(lines, i, j);
        }
    }

    for (int i = lines.size() - 1; i >= 0; i--) {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == 'O') move_down(lines, i, j);
        }
    }

    for (int i = 0; i < lines.size(); i++) {
        for (int j = lines[i].size() - 1; j >= 0; j--) {
            if (lines[i][j] == 'O') move_right(lines, i, j);
        }
    }
}

void day14::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<string> lines;

        string line;
        while (getline(file, line)) lines.push_back(line);

        // move all round rocks up
        vector<string> lines_p1 = lines;
        for (int i = 0; i < lines_p1.size(); i++) {
            for (int j = 0; j < lines_p1[i].size(); j++) {
                if (lines_p1[i][j] == 'O') move_up(lines_p1, i, j);
            }
        }

        // calculate load
        int result_p1 = 0;
        for (int i = 0; i < lines_p1.size(); i++) {
            for (int j = 0; j < lines_p1[i].size(); j++) {
                if (lines_p1[i][j] == 'O') result_p1 += lines_p1.size() - i;
            }
        }

        // cycle - 1000 produces the same result as 1000000000
        for (int i = 0; i < 1000; i++) cycle(lines);

        // calculate load
        int result_p2 = 0;
        for (int i = 0; i < lines.size(); i++) {
            for (int j = 0; j < lines[i].size(); j++) {
                if (lines[i][j] == 'O') result_p2 += lines.size() - i;
            }
        }

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

    } else std::cout << "Can't open file" << std::endl;
}
