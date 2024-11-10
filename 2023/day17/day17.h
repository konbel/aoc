//
// Created by Konstantin Kilbel on 17.12.23.
//

#ifndef DAY17_H
#define DAY17_H

namespace day17 {
    struct State {
        int hl, r, c, dr, dc, n;

        bool operator>(const State& other) const {
            return hl > other.hl;
        }
    };

    int findPathP1(const vector<string>& map);
    int findPathP2(const vector<string>& map);
    void solve(const string& input);
}

#endif //DAY17_H