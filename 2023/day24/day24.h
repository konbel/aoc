//
// Created by Konstantin Kilbel on 24.12.23.
//

#ifndef DAY24_H
#define DAY24_H

namespace day24 {
    struct Hailstone {
        long sx, sy, sz;
        float vx, vy, vz;
        float a, b, c;

        Hailstone (const vector<long>& startPosition, const vector<float>& velocity) {
            sx = startPosition[0];
            sy = startPosition[1];
            sz = startPosition[2];

            vx = velocity[0];
            vy = velocity[1];
            vz = velocity[2];

            a = vy;
            b = -vx;
            c = vy * sx - vx * sy;
        }
    };

    void solve(const string& input);
}

#endif //DAY24_H