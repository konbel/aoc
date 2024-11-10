//
// Created by Konstantin Kilbel on 22.12.23.
//

#include "../includes.h"
#include "day22.h"

#include <deque>
#include <set>

using day22::Block;

bool day22::overlaps(const Block& b1, const Block& b2) {
    return std::max(b1.start.x, b2.start.x) <= std::min(b1.end.x, b2.end.x) && std::max(b1.start.y, b2.start.y) <= std::min(b1.end.y, b2.end.y);
}

vector<int> day22::disintegrate(const vector<Block>& blocks) {
    vector<Block> blocksCopy = blocks;
    std::ranges::sort(blocksCopy, [](const Block& a, const Block& b) { return a.start.z < b.start.z; });

    for (int i = 0; i < blocks.size(); i++) {
        Block& b1 = blocksCopy[i];

        if (b1.start.z == 1 || b1.end.z == 1) continue;

        int zAfterFall = 1;

        for (int j = 0; j < i; j++) {
            const Block& b2 = blocksCopy[j];

            if (overlaps(b1, b2)) zAfterFall = std::max(zAfterFall, b2.end.z + 1);
        }

        b1.end.z -= b1.start.z - zAfterFall;
        b1.start.z = zAfterFall;
    }

    std::ranges::sort(blocksCopy, [](const Block& a, const Block& b) { return a.start.z < b.start.z; });

    map<int, vector<int>> k_supports_v;
    map<int, vector<int>> v_supports_k;

    for (int i = 0; i < blocksCopy.size(); i++) {
        k_supports_v[i] = {};
        v_supports_k[i] = {};
    }

    for (int i = 0; i < blocksCopy.size(); i++) {
        const Block& b1 = blocksCopy[i];
        for (int j = 0; j < i; j++) {
            const Block& b2 = blocksCopy[j];
            if (overlaps(b1, b2) && b1.start.z == b2.end.z + 1) {
                k_supports_v[j].push_back(i);
                v_supports_k[i].push_back(j);
            }
        }
    }

    int canDisintegrate = 0;
    int wouldFall = 0;
    for (int i = 0; i < blocksCopy.size(); i++) {
        int multiple = 0;
        std::set<int> falling;
        std::deque<int> queue;
        for (int j : k_supports_v[i]) {
            if (v_supports_k[j].size() >= 2) multiple++;
            else if (v_supports_k[j].size() == 1) {
                falling.insert(j);
                queue.push_back(j);
            }
        }

        if (multiple == k_supports_v[i].size()) canDisintegrate += 1;

        while (!queue.empty()) {
            int j = queue.front();
            queue.pop_front();
            for (int k : k_supports_v[j]) {
                if (falling.contains(k)) continue;

                int alreadyFalling = 0;
                for (int l : v_supports_k[k]) {
                    if (falling.contains(l)) alreadyFalling++;
                }
                if (alreadyFalling == v_supports_k[k].size()) {
                    queue.push_back(k);
                    falling.insert(k);
                }
            }
        }

        wouldFall += falling.size();
    }

    return {canDisintegrate, wouldFall};
}

void day22::solve(string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        vector<Block> blocks;

        string line;
        while (getline(file, line)) {
            int seperator = line.find('~');
            string vStr1 = line.substr(0, seperator) + ',';
            string vStr2 = line.substr(seperator + 1, line.size()) + ',';

            vector<string> nums;
            string num;
            for (char c : vStr1) {
                if (c == ',') {
                    nums.push_back(num);
                    num.clear();
                } else num += c;
            }
            Block block;
            block.start.x = std::stoi(nums[0]);
            block.start.y = std::stoi(nums[1]);
            block.start.z = std::stoi(nums[2]);

            nums.clear();
            num.clear();
            for (char c : vStr2) {
                if (c == ',') {
                    nums.push_back(num);
                    num.clear();
                } else num += c;
            }
            block.end.x = std::stoi(nums[0]);
            block.end.y = std::stoi(nums[1]);
            block.end.z = std::stoi(nums[2]);

            blocks.push_back(block);
        }

        vector results = disintegrate(blocks);

        cout << "Solution problem 1:" << results[0] << endl;
        cout << "Solution problem 1:" << results[1] << endl;

        file.close();
    } else cout << "Can't open file" << endl;
}