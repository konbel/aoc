#include "../includes.h"
#include "day22.h"

struct Vector3D {
    int x, y, z;

    Vector3D operator+ (const Vector3D& v2) const {
        Vector3D v3 = *this;
        v3.x += v2.x;
        v3.y += v2.y;
        v3.z += v2.z;
        return v3;
    }

    bool operator== (const Vector3D& v2) const {
        return this->x == v2.x && this->y == v2.y && this->z == v2.z;
    }

    friend std::ostream& operator<<(std::ostream& output, const Vector3D& v) {
        output << "{" << v.x << ", " << v.y << ", " << v.z << "}";
        return output;
    }
};

struct Block {
    Vector3D start, end;

    bool operator== (const Block& b2) const {
        return this->start == b2.start && this->end == b2.end;
    }

    friend std::ostream& operator<< (std::ostream& output, const Block& b) {
        output << b.start << b.end;
        return output;
    }
};

static bool overlaps(const Block& b1, const Block& b2) {
    return std::max(b1.start.x, b2.start.x) <= std::min(b1.end.x, b2.end.x) && std::max(b1.start.y, b2.start.y) <= std::min(b1.end.y, b2.end.y);
}

static vector<int> disintegrate(const vector<Block>& blocks) {
    vector<Block> blocks_copy = blocks;
    std::ranges::sort(blocks_copy, [](const Block& a, const Block& b) { return a.start.z < b.start.z; });

    for (int i = 0; i < blocks.size(); i++) {
        Block& b1 = blocks_copy[i];

        if (b1.start.z == 1 || b1.end.z == 1) continue;

        int z_after_fall = 1;

        for (int j = 0; j < i; j++) {
            const Block& b2 = blocks_copy[j];

            if (overlaps(b1, b2)) z_after_fall = std::max(z_after_fall, b2.end.z + 1);
        }

        b1.end.z -= b1.start.z - z_after_fall;
        b1.start.z = z_after_fall;
    }

    std::ranges::sort(blocks_copy, [](const Block& a, const Block& b) { return a.start.z < b.start.z; });

    map<int, vector<int>> k_supports_v;
    map<int, vector<int>> v_supports_k;

    for (int i = 0; i < blocks_copy.size(); i++) {
        k_supports_v[i] = {};
        v_supports_k[i] = {};
    }

    for (int i = 0; i < blocks_copy.size(); i++) {
        const Block& b1 = blocks_copy[i];
        for (int j = 0; j < i; j++) {
            const Block& b2 = blocks_copy[j];
            if (overlaps(b1, b2) && b1.start.z == b2.end.z + 1) {
                k_supports_v[j].push_back(i);
                v_supports_k[i].push_back(j);
            }
        }
    }

    int can_disintegrate = 0;
    int would_fall = 0;
    for (int i = 0; i < blocks_copy.size(); i++) {
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

        if (multiple == k_supports_v[i].size()) can_disintegrate += 1;

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

        would_fall += falling.size();
    }

    return {can_disintegrate, would_fall};
}

void day22::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<Block> blocks;

        string line;
        while (getline(file, line)) {
            int seperator = line.find('~');
            string v_str_1 = line.substr(0, seperator) + ',';
            string v_str_2 = line.substr(seperator + 1, line.size()) + ',';

            vector<string> nums;
            string num;
            for (char c : v_str_1) {
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
            for (char c : v_str_2) {
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

        std::cout << "Solution problem 1:" << results[0] << std::endl;
        std::cout << "Solution problem 1:" << results[1] << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}