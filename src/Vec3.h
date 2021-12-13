#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <iomanip>
//#include "glm/vec3.hpp"

class Vec3
{
public:
    Vec3() { e[0] = 0; e[1] = 0; e[2] = 0; } // Init to zero vector
    Vec3(double e0, double e1, double e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    ~Vec3() {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Operator Overloads
    Vec3 operator+(const Vec3& v2) const;
    Vec3 operator-(const Vec3& v2) const;
    Vec3 operator*(const Vec3& v2) const;
    Vec3 operator/(const Vec3& v2) const;

    // Friendly Overloads
    friend Vec3 operator*(double lhs, const Vec3& rhs);
    friend Vec3 operator*(const Vec3& lhs, double rhs);
    friend Vec3 operator/(double t, const Vec3& rhs);
    friend Vec3 operator/(const Vec3& lhs, double t);

    inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

    // TODO: Unary Negation
    // TODO: Look up proper way to do operator[] overloading

    Vec3& operator+=(const Vec3& v2);
    Vec3& operator-=(const Vec3& v2);
    Vec3& operator*=(const Vec3& v2);
    Vec3& operator/=(const Vec3& v2);
    Vec3& operator*=(const double t);
    Vec3& operator/=(const double t);

    double length() const;
    double squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
    void make_unit_vector();
    static Vec3 unit_vector(Vec3 v);

    // Static Members
    static double dot(const Vec3& v1, const Vec3& v2);
    static Vec3 cross(const Vec3& v1, const Vec3& v2);

    double e[3];

    // Util
    std::string toString();
private:
};