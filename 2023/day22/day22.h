//
// Created by Konstantin Kilbel on 22.12.23.
//

#ifndef DAY22_H
#define DAY22_H

namespace day22 {
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

    bool overlaps(const Block& b1, const Block& b2);
    vector<int> disintegrate(const vector<Block>& blocks);
    void solve(string input);
}

#endif //DAY22_H