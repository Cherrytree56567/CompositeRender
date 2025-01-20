#include "Vec3.h"

Vec3 stringToVec3(const std::string& str) {
    Vec3 vec;
    std::stringstream ss(str);

    ss >> vec.x;
    ss.ignore(1, ',');
    ss >> vec.y;
    ss.ignore(1, ',');
    ss >> vec.z;

    return vec;
}