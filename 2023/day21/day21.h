//
// Created by Konstantin Kilbel on 21.12.23.
//

#ifndef DAY21_H
#define DAY21_H

namespace day21 {
    vector<int> addVector(const vector<int>& v1, const vector<int>& v2);
    bool checkBounds(const int& maxX, const int& maxY, const vector<int>& position);
    int takeSteps(vector<string>& garden, const vector<int>& startPosition, const int& stepsToTake);
    long long calcSteps(vector<string>& garden, const vector<int>& startPosition, const int& stepsToTake);
    void solve(const string& input);
}

#endif //DAY21_H