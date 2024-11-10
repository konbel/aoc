//
// Created by Konstantin Kilbel on 16.12.23.
//

#ifndef DAY16_H
#define DAY16_H

namespace day16 {
    struct Ray {
        vector<int> position;
        vector<int> direction;
    };

    int shootRay(const vector<string>& grid, const vector<int>& startPosition, const vector<int>& direction);
    void solve(const string& input);
}

#endif //DAY16_H