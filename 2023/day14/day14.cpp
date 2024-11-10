//
// Created by Konstantin Kilbel on 14.12.23.
//

#include "../includes.h"
#include "day14.h"

void day14::moveUp(vector<string>& lines, int currenLine, int currentChar) {
    if (currenLine - 1 < 0) return;
    if (lines[currenLine - 1][currentChar] == '.') {
        lines[currenLine][currentChar] = '.';
        lines[currenLine - 1][currentChar] = 'O';
        moveUp(lines, currenLine - 1, currentChar);
    }
}

void day14::moveLeft(vector<string>& lines, int currenLine, int currentChar) {
    if (currentChar - 1 < 0) return;
    if (lines[currenLine][currentChar - 1] == '.') {
        lines[currenLine][currentChar] = '.';
        lines[currenLine][currentChar - 1] = 'O';
        moveLeft(lines, currenLine, currentChar - 1);
    }
}

void day14::moveDown(vector<string>& lines, int currenLine, int currentChar) {
    if (currenLine + 1 >= lines.size()) return;
    if (lines[currenLine + 1][currentChar] == '.') {
        lines[currenLine][currentChar] = '.';
        lines[currenLine + 1][currentChar] = 'O';
        moveDown(lines, currenLine + 1, currentChar);
    }
}

void day14::moveRight(vector<string>& lines, int currenLine, int currentChar) {
    if (currentChar + 1 >= lines[currenLine].size()) return;
    if (lines[currenLine][currentChar + 1] == '.') {
        lines[currenLine][currentChar] = '.';
        lines[currenLine][currentChar + 1] = 'O';
        moveRight(lines, currenLine, currentChar + 1);
    }
}

void day14::cycle(vector<string>& lines) {
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == 'O') moveUp(lines, i, j);
        }
    }

    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == 'O') moveLeft(lines, i, j);
        }
    }

    for (int i = lines.size() - 1; i >= 0; i--) {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == 'O') moveDown(lines, i, j);
        }
    }

    for (int i = 0; i < lines.size(); i++) {
        for (int j = lines[i].size() - 1; j >= 0; j--) {
            if (lines[i][j] == 'O') moveRight(lines, i, j);
        }
    }
}

void day14::solve(string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        vector<string> lines;

        string line;
        while (getline(file, line)) lines.push_back(line);

        // move all round rocks up
        vector<string> linesP1 = lines;
        for (int i = 0; i < linesP1.size(); i++) {
            for (int j = 0; j < linesP1[i].size(); j++) {
                if (linesP1[i][j] == 'O') moveUp(linesP1, i, j);
            }
        }

        // calculate load
        int resultP1 = 0;
        for (int i = 0; i < linesP1.size(); i++) {
            for (int j = 0; j < linesP1[i].size(); j++) {
                if (linesP1[i][j] == 'O') resultP1 += linesP1.size() - i;
            }
        }

        // cycle - 1000 produces the same result as 1000000000
        for (int i = 0; i < 1000; i++) cycle(lines);

        // calculate load
        int resultP2 = 0;
        for (int i = 0; i < lines.size(); i++) {
            for (int j = 0; j < lines[i].size(); j++) {
                if (lines[i][j] == 'O') resultP2 += lines.size() - i;
            }
        }

        cout << "Solution problem 1: " << resultP1 << endl;
        cout << "Solution problem 2: " << resultP2 << endl;

    } else cout << "Can't open file" << endl;
}
