#include "../includes.h"
#include "day24.h"

void day24::solve(const string& input) {
    if (std::ifstream file(input); file.is_open()) {
        vector<Hailstone> hailstones;

        string line;
        while (getline(file, line)) {
            const int seperator = line.find('@');

            string pos_string = line.substr(0, seperator - 1) + ',';
            vector<long> position;
            string num;
            for (int i = 0; i < pos_string.size(); i++) {
                if (pos_string[i] == ',') {
                    position.push_back(std::stol(num));
                    num.clear();
                    i++;
                }
                num += pos_string[i];
            }

            vector<float> velocity;
            string vel_string = line.substr(seperator + 2, line.size() - 1) + ',';
            num.clear();
            for (int i = 0; i < vel_string.size(); i++) {
                if (vel_string[i] == ',') {
                    velocity.push_back(std::stof(num));
                    num.clear();
                    i++;
                }
                num += vel_string[i];
            }

            hailstones.push_back(Hailstone(position, velocity));
        }

        constexpr long min = 200'000'000'000'000;
        constexpr long max = 400'000'000'000'000;
        
        int result_p1 = 0;
        for (int i = 0; i < hailstones.size(); i++) {
            const Hailstone& h1 = hailstones[i];
            for (int j = i + 1; j < hailstones.size(); j++) {
                const Hailstone& h2 = hailstones[j];
                if (h1.a * h2.b == h2.a * h1.b) continue;

                const float x = (h1.c * h2.b - h2.c * h1.b) / (h1.a * h2.b - h2.a * h1.b);
                const float y = (h2.c * h1.a - h1.c * h2.a ) / (h1.a * h2.b - h2.a * h1.b);

                if (x >= min && x <= max && y >= min && y <= max) {
                    if ((x - h1.sx) * h1.vx >= 0 && (y - h1.sy) * h1.vy >= 0 &&
                        (x - h2.sx) * h2.vx >= 0 && (y - h2.sy) * h2.vy >= 0) {
                        result_p1++;
                    }
                }
            }
        }

        // part two is solved in python script, too lazy to programm an algorith to solve the equations

        std::cout << "Solution problem 1: " << result_p1 << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}