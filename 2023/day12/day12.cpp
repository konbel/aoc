//
// Created by Konstantin Kilbel on 12.12.23.
//

#include "../includes.h"
#include "day12.h"
#include <unordered_map>

std::unordered_map<string, long> cacheP1;
std::unordered_map<string, long> cacheP2;

long day12::countArrangements(string springs, std::deque<long> nums, bool p2) {
    if (springs.empty()) {
        if (nums.empty()) return 1;
        return 0;
    }

    if (nums.empty()) {
        if (springs.find('#') == string::npos) return 1;
        return 0;
    }

    string key = springs;
    for (const long i : nums) key += std::to_string(i) + "_";

    auto& currentCache = p2 ? cacheP2 : cacheP1;

    if (currentCache.contains(key)) return currentCache[key];

    long result = 0;

    if (springs[0] == '.' || springs[0] == '?') {
        result += countArrangements(springs.substr(1, springs.size() - 1), nums, p2);
    }

    if (springs[0] == '#' || springs[0] == '?') {
        if (nums[0] <= springs.size() && springs.substr(0, nums[0]).find('.') == string::npos && (nums[0] == springs.size() || springs[nums[0]] != '#')) {
            std::deque<long> copyNums = nums;
            copyNums.pop_front();
            const string copySprings = nums[0] < springs.size() ? springs.substr(nums[0] + 1, springs.size() - nums[0] + 1) : "";
            result += countArrangements(copySprings, copyNums, p2);
        }
    }

    currentCache.insert(std::pair<string, long>(key, result));

    return result;
}


void day12::solve(string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        long resultP1;
        long resultP2;

        string line;
        while (getline(file, line)) {
            int seperator = line.find(" ");
            string springs = line.substr(0, seperator);
            string counts = line.substr(seperator + 1, line.size() - 1);
            counts += ',';

            std::deque<long> springCount;
            string num;
            for (int i = 0; i < counts.size(); i++) {
                char c = counts[i];
                if (c == ',') {
                    springCount.push_back(std::stol(num));
                    num.clear();
                } else num += c;
            }

            resultP1 += countArrangements(springs, springCount, false);

            string springsP2;
            std::deque<long> springCountP2;
            for (int i = 0; i < 5; i++) {
                springsP2 += springs + '?';
                for (long j : springCount) springCountP2.push_back(j);
            }
            springsP2.pop_back();

            resultP2 += countArrangements(springsP2, springCountP2, true);
        }

        cout << "Solution problem 1: " << resultP1 << endl;
        cout << "Solution problem 2: " << resultP2 << endl;

    } else cout << "Can't open file" << endl;
}