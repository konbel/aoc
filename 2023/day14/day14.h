//
// Created by Konstantin Kilbel on 14.12.23.
//

#ifndef DAY14_H
#define DAY14_H

namespace day14 {
    void moveUp(vector<string>& lines, int currentLine, int currentChar);
    void moveLeft(vector<string>& lines, int currenLine, int currentChar);
    void moveDown(vector<string>& lines, int currenLine, int currentChar);
    void moveRight(vector<string>& lines, int currenLine, int currentChar);
    void cycle(vector<string>& lines);
    void solve(string input);
}

#endif //DAY14_H