#pragma once
#include <cmath>

class Vector2D {
public:
    float x, y;

    // >> Constructor overloading << //
    Vector2D() : x(0), y(0) {}
    Vector2D(float x, float y) : x(x), y(y) {}

    // >> Operator overloading << //
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D& operator+=(const Vector2D& other) {
		this->x += other.x;         // >> this pointer << //
        this->y += other.y;
        return *this;
    }

    // >> Inline functions << //
    inline float length() const {
        return std::sqrt(x * x + y * y);
    }

    inline Vector2D normalize() const {
        float len = length();
        if (len > 0) {
            return Vector2D(x / len, y / len);
        }
        return Vector2D(0, 0);
    }
};