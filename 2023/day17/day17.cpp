#include "../includes.h"
#include "day17.h"

struct State {
    int hl, r, c, dr, dc, n;

    bool operator>(const State& other) const {
        return hl > other.hl;
    }
};

static int find_path_p1(const vector<string>& map) {
    std::set<std::tuple<int, int, int, int, int>> seen;
    std::priority_queue<State, vector<State>, std::greater<State>> pq;
    pq.push({0, 0, 0, 0, 0, 0});

    while (!pq.empty()) {
        auto [hl, r, c, dr, dc, n] = pq.top();
        pq.pop();

        if (r == map.size() - 1 && c == map[0].size() - 1) return hl;

        if (seen.contains({r, c, dr, dc, n})) continue;

        seen.insert({r, c, dr, dc, n});

        if (n < 3 && (dr != 0 || dc != 0)) {
            const int nr = r + dr;
            const int nc = c + dc;
            if (0 <= nr && nr < map.size() && 0 <= nc && nc < map[0].size()) pq.push({hl + (map[nr][nc] - '0'), nr, nc, dr, dc, n + 1});
        }

        for (auto [ndr, ndc] : vector<pair<int, int>>{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}) {
            if ((ndr != dr || ndc != dc) && (ndr != -dr || ndc != -dc)) {
                const int nr = r + ndr;
                const int nc = c + ndc;
                if (0 <= nr && nr < map.size() && 0 <= nc && nc < map[0].size()) pq.push({hl + (map[nr][nc] - '0'), nr, nc, ndr, ndc, 1});
            }
        }
    }

    return 0;
}

static int find_path_p2(const vector<string>& map) {
    std::set<std::tuple<int, int, int, int, int>> seen;
    std::priority_queue<State, vector<State>, std::greater<State>> pq;
    pq.push({0, 0, 0, 0, 0, 0});

    while (!pq.empty()) {
        auto [hl, r, c, dr, dc, n] = pq.top();
        pq.pop();

        if (r == map.size() - 1 && c == map[0].size() - 1 && n >= 4) return hl;

        if (seen.contains({r, c, dr, dc, n})) continue;

        seen.insert({r, c, dr, dc, n});

        if (n < 10 && (dr != 0 || dc != 0)) {
            const int nr = r + dr;
            const int nc = c + dc;
            if (0 <= nr && nr < map.size() && 0 <= nc && nc < map[0].size()) pq.push({hl + (map[nr][nc] - '0'), nr, nc, dr, dc, n + 1});
        }

        if (n >= 4 || (dr == 0 && dc == 0)) {
            for (auto [ndr, ndc] : vector<pair<int, int>>{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}) {
                if ((ndr != dr || ndc != dc) && (ndr != -dr || ndc != -dc)) {
                    const int nr = r + ndr;
                    const int nc = c + ndc;
                    if (0 <= nr && nr < map.size() && 0 <= nc && nc < map[0].size()) pq.push({hl + (map[nr][nc] - '0'), nr, nc, ndr, ndc, 1});
                }
            }
        }
    }

    return 0;
}

void day17::solve(const string& input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<string> map;

        string line;
        while (getline(file, line)) map.push_back(line);

        const int result_p1 = find_path_p1(map);
        const int result_p2 = find_path_p2(map);

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}
