//
// Created by Konstantin Kilbel on 23.12.23.
//

#ifndef DAY23_H
#define DAY23_H

#include <set>

namespace day23 {
    struct Step {
        Step (const Step& s) {
            curPosition = s.curPosition;
            prevPosition = s.prevPosition;
        }

        Step (const vector<int>& startPosition) {
            curPosition = startPosition;
            prevPosition = startPosition;
        }

        bool operator==(const Step& s) const {
            return curPosition == s.curPosition;
        }

        bool operator<(const Step& s) const {
            return curPosition < s.curPosition;
        }

        vector<int> curPosition;
        vector<int> prevPosition;
    };

    struct Path {
        Path (const Path& p) {
            steps = p.steps;
            seen = p.seen;
        }

        Path (const Step& startPosition) {
            steps.push_back(startPosition);
            seen.insert(startPosition.curPosition);
        }

        vector<Step> steps;
        std::set<vector<int>> seen;
    };

    int dfs(map<vector<int>, map<vector<int>, int>>& graph, const vector<int>& p, std::set<vector<int>>& seen, const vector<int>& end);
    int getLongestPath(const vector<string>& grid);
    void solve(const string& input);
}

#endif //DAY23_H