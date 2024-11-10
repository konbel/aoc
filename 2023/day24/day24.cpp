//
// Created by Konstantin Kilbel on 24.12.23.
//

#include "../includes.h"
#include "day24.h"

void day24::solve(const string& input) {
    std::ifstream file(input);
    if (file.is_open()) {
        vector<Hailstone> hailstones;

        string line;
        while (getline(file, line)) {
            const int seperator = line.find('@');

            string posString = line.substr(0, seperator - 1) + ',';
            vector<long> position;
            string num;
            for (int i = 0; i < posString.size(); i++) {
                if (posString[i] == ',') {
                    position.push_back(std::stol(num));
                    num.clear();
                    i++;
                }
                num += posString[i];
            }

            vector<float> velocity;
            string velString = line.substr(seperator + 2, line.size() - 1) + ',';
            num.clear();
            for (int i = 0; i < velString.size(); i++) {
                if (velString[i] == ',') {
                    velocity.push_back(std::stof(num));
                    num.clear();
                    i++;
                }
                num += velString[i];
            }

            hailstones.push_back(Hailstone(position, velocity));
        }

        constexpr long min = 200'000'000'000'000;
        constexpr long max = 400'000'000'000'000;
        
        int resultP1 = 0;
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
                        resultP1++;
                    }
                }
            }
        }

        // part two is solved in python script, too lazy to programm an algorith to solve the equations

        cout << "Solution problem 1: " << resultP1 << endl;

        file.close();
    } else cout << "Can't open file" << endl;
}