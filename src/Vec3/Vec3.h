#pragma once
#include <algorithm>
#include <string>
#include <sstream>

struct Vec3 {
    float x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float r, float g, float b) : x(r), y(g), z(b) {}
    
    Vec3 operator*(float value) const {
        return Vec3(x * value, y * value, z * value);
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    
    Vec3 clamp() const {
        return Vec3(std::clamp(x, 0.0f, 1.0f), std::clamp(y, 0.0f, 1.0f), std::clamp(z, 0.0f, 1.0f));
    }
};

Vec3 stringToVec3(const std::string& str);