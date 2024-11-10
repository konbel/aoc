//
// Created by Konstantin Kilbel on 23.12.23.
//

#include "../includes.h"
#include "day23.h"

#include <queue>

int day23::getLongestPath(const vector<string>& grid) {
    const vector start = {static_cast<int>(grid[0].find('.')), 0};
    const vector end = {static_cast<int>(grid[static_cast<int>(grid.size() - 1)].find('.')), static_cast<int>(grid.size() - 1)};

    vector points = {start, end};

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (i < 0 && i >= grid.size() && j < 0 && j >= grid[0].size()) continue;
            if (grid[i][j] == '#') continue;

            int neighbours = 0;
            for (vector<int> d : vector<vector<int>>{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}) {
                if (i + d[1] >= 0 && i + d[0] < grid.size() && j + d[0] >= 0 && j + d[0] < grid[0].size()) {
                    if (grid[i + d[1]][j + d[0]] != '#') neighbours++;
                }
            }

            if (neighbours >= 3) points.push_back({j, i});
        }
    }

    map<vector<int>, map<vector<int>, int>> graph;
    for (vector<int> p : points) graph[p] = {};

    map<char, vector<vector<int>>> dirs;
    dirs.insert(std::pair<char, vector<vector<int>>>('>', {{1, 0}}));
    dirs.insert(std::pair<char, vector<vector<int>>>('v', {{0, 1}}));
    dirs.insert(std::pair<char, vector<vector<int>>>('<', {{-1, 0}}));
    dirs.insert(std::pair<char, vector<vector<int>>>('.', {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}));

    for (vector<int> p : points) {
        vector<vector<int>> stack = {{0, p[0], p[1]}};
        std::set<vector<int>> seen;
        seen.insert({p[0], p[1]});

        while (!stack.empty()) {
            const int n = stack[stack.size() - 1][0];
            const int c = stack[stack.size() - 1][1];
            const int r = stack[stack.size() - 1][2];
            stack.pop_back();

            if (n != 0 && graph.contains({c, r})) {
                graph[p][vector{c, r}] = n;
                continue;
            }

            for (vector<int> d : dirs[grid[r][c]]) {
                const int nr = r + d[1];
                const int nc = c + d[0];
                if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[0].size() && grid[nr][nc] != '#' && !seen.contains({nc, nr})) {
                    stack.push_back({n + 1, nc, nr});
                    seen.insert({nc, nr});
                }
            }
        }
    }

    std::set<vector<int>> seen;

    return dfs(graph, start, seen, end);
}

int day23::dfs(map<vector<int>, map<vector<int>, int>>& graph, const vector<int>& p, std::set<vector<int>>& seen, const vector<int>& end) {
    if (p == end) return 0;

    int max = std::numeric_limits<int>::min();

    seen.insert(p);
    for (const auto& [nx, _] : graph[p]) {
        if (seen.contains(nx)) continue;
        max = std::max(max, dfs(graph, nx, seen, end) + graph[p][nx]);
    }
    seen.erase(p);

    return max;
}

void day23::solve(const string& input) {
    std::ifstream file(input);
    if (file.is_open()) {
        vector<string> grid;
        string line;
        while (getline(file, line)) grid.push_back(line);

        const int resultP1 = getLongestPath(grid);

        vector<string> gridNoSlopes = grid;
        for (string& l : gridNoSlopes) {
            for (char& c : l) {
                if (c != '#' && c != '.') c = '.';
            }
        }
        const int resultP2 = getLongestPath(gridNoSlopes);

        cout << "Solution problem 1: " << resultP1 << endl;
        cout << "Solution problem 2: " << resultP2 << endl;

        file.close();
    } else cout << "Can't open file" << endl;
}