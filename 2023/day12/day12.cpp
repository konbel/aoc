#include "../includes.h"
#include "day12.h"

std::unordered_map<string, long> cache_p1;
std::unordered_map<string, long> cache_p2;

static long count_arrangements(string springs, std::deque<long> nums, bool p2) {
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

    auto& current_cache = p2 ? cache_p2 : cache_p1;

    if (current_cache.contains(key)) return current_cache[key];

    long result = 0;

    if (springs[0] == '.' || springs[0] == '?') {
        result += count_arrangements(springs.substr(1, springs.size() - 1), nums, p2);
    }

    if (springs[0] == '#' || springs[0] == '?') {
        if (nums[0] <= springs.size() && springs.substr(0, nums[0]).find('.') == string::npos && (nums[0] == springs.size() || springs[nums[0]] != '#')) {
            std::deque<long> copy_nums = nums;
            copy_nums.pop_front();
            const string copy_strings = nums[0] < springs.size() ? springs.substr(nums[0] + 1, springs.size() - nums[0] + 1) : "";
            result += count_arrangements(copy_strings, copy_nums, p2);
        }
    }

    current_cache.insert(pair<string, long>(key, result));

    return result;
}


void day12::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        long result_p1;
        long result_p2;

        string line;
        while (getline(file, line)) {
            int seperator = line.find(" ");
            string springs = line.substr(0, seperator);
            string counts = line.substr(seperator + 1, line.size() - 1);
            counts += ',';

            std::deque<long> spring_count;
            string num;
            for (int i = 0; i < counts.size(); i++) {
                char c = counts[i];
                if (c == ',') {
                    spring_count.push_back(std::stol(num));
                    num.clear();
                } else num += c;
            }

            result_p1 += count_arrangements(springs, spring_count, false);

            string springs_p2;
            std::deque<long> spring_count_p2;
            for (int i = 0; i < 5; i++) {
                springs_p2 += springs + '?';
                for (long j : spring_count) spring_count_p2.push_back(j);
            }
            springs_p2.pop_back();

            result_p2 += count_arrangements(springs_p2, spring_count_p2, true);
        }

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

    } else std::cout << "Can't open file" << std::endl;
}