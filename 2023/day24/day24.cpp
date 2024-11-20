#include "../includes.h"
#include "day24.h"
#include <cassert>
#include <__numeric/gcd_lcm.h>

using Vec = std::array<long, 3>;
using Hailstone = pair<Vec, Vec>;

Vec operator+(Vec const &v1, Vec const &v2) {
    return {v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]};
}

Vec operator-(Vec const &v1, Vec const &v2) {
    return {v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]};
}

Vec operator*(long sc, Vec const &v) {
    return {sc * v[0], sc * v[1], sc * v[2]};
}

Vec operator/(Vec const &v, long sc) {
    assert(v[0] % sc == 0 && v[1] % sc == 0 && v[2] % sc == 0);
    return {v[0] / sc, v[1] / sc, v[2] / sc};
}

int nonzero_index(Vec const &v) {
    assert(v != Vec({0, 0, 0}));
    return v[0] != 0 ? 0 : (v[1] != 0 ? 1 : 2);
}

Vec direction(Vec const &v) {
    long d = std::gcd(std::gcd(v[0], v[1]), v[2]);
    if (d == 0)
        return v;
    return {v[0] / d, v[1] / d, v[2] / d};
}

Vec directional_cross(Vec a, Vec b) {
    a = direction(a);
    b = direction(b);
    Vec c;
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
    return direction(c);
}

long dot(Vec const &a, Vec const &b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


pair<Vec, Vec> thread_the_needle(Hailstone &h1, Hailstone &h2, Hailstone &h3) {
    auto [p1, v1] = h1;
    auto [p2, v2] = h2;
    auto [p3, v3] = h3;

    p2 = p2 - p1;
    v2 = v2 - v1;
    p3 = p3 - p1;
    v3 = v3 - v1;

    Vec n2 = directional_cross(p2, p2 + v2);
    Vec n3 = directional_cross(p3, p3 + v3);

    Vec v = directional_cross(n2, n3);

    if (v == Vec{0, 0, 0}) return {v, v};

    auto vv = dot(v, v);
    auto hit_time = [&](Vec hp, Vec hv) {
        hp = vv * hp - dot(v, hp) * v;
        hv = vv * hv - dot(v, hv) * v;

        int x = nonzero_index(hv);
        assert(hp[x] % hv[x] == 0);

        return -hp[x] / hv[x];
    };

    auto t2 = hit_time(p2, v2);
    auto t3 = hit_time(p3, v3);

    if (t2 == t3) return {Vec{0, 0, 0}, Vec{0, 0, 0}};

    assert(t2 >= 0 && t3 >= 0 && t2 != t3);

    auto hit_pos2 = h2.first + t2 * h2.second;
    auto hit_pos3 = h3.first + t3 * h3.second;

    v = (hit_pos3 - hit_pos2) / (t3 - t2);

    return {hit_pos2 - t2 * v, v};
}

void day24::solve(const string& input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<Hailstone> hailstones;

        string line;
        while (getline(file, line)) {
            const int seperator = line.find('@');

            string pos_string = line.substr(0, seperator - 1) + ',';
            Vec position;
            string num;
            int index = 0;
            for (int i = 0; i < pos_string.size(); i++) {
                if (pos_string[i] == ',') {
                    position[index] = std::stol(num);
                    num.clear();
                    index++;
                    i++;
                }
                num += pos_string[i];
            }

            Vec velocity;
            string vel_string = line.substr(seperator + 2, line.size() - 1) + ',';
            num.clear();
            index = 0;
            for (int i = 0; i < vel_string.size(); i++) {
                if (vel_string[i] == ',') {
                    velocity[index] = std::stol(num);
                    num.clear();
                    index++;
                    i++;
                }
                num += vel_string[i];
            }

            hailstones.emplace_back(position, velocity);
        }

        constexpr long min = 200'000'000'000'000;
        constexpr long max = 400'000'000'000'000;
        
        int result_p1 = 0;
        for (int i = 0; i < hailstones.size(); i++) {
            const auto &[p1, v1] = hailstones[i];
            long sx1 = p1[0], sy1 = p1[1];
            long vx1 = v1[0];
            long vy1 = v1[1];
            long a1 = vy1;
            long b1 = -vx1;
            float c1 = vy1 * sx1 - vx1 * sy1;


            for (int j = i + 1; j < hailstones.size(); j++) {
                const auto &[p2, v2] = hailstones[j];
                long sx2 = p2[0], sy2 = p2[1];
                long vx2 = v2[0];
                long vy2 = v2[1];

                long a2 = vy2;
                long b2 = -vx2;
                float c2 = vy2 * sx2 - vx2 * sy2;

                if (a1 * b2 == a2 * b1) continue;

                long x = (c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1);
                long y = (c2 * a1 - c1 * a2) / (a1 * b2 - a2 * b1);

                if (x >= min && x <= max && y >= min && y <= max) {
                    if ((x - sx1) * vx1 >= 0 && (y - sy1) * vy1 >= 0 && (x - sx2) * vx2 >= 0 && (y - sy2) * vy2 >= 0) {
                        result_p1++;
                    }
                }
            }
        }

        auto [p, v] = thread_the_needle(hailstones[0], hailstones[1], hailstones[2]);

        assert(v != Vec({ 0, 0, 0 }));

        auto is_hit = [&](Hailstone const &h) {
            auto [hp, hv] = h;
            auto dp = p - hp;
            auto dv = hv - v;
            int x = nonzero_index(dv);
            auto t = dp[x] / dv[x];
            return hp + t * hv == p + t * v;
        };

        for (auto const &h : hailstones) assert(is_hit(h));

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << p[0] + p[1] + p[2] << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}