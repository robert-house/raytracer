#include "Vec3.h"

// Non-Member operator overloads
Vec3 operator*(double lhs, const Vec3& rhs)
{
    return Vec3(lhs * rhs.e[0],
        lhs * rhs.e[1],
        lhs * rhs.e[2]);
}

Vec3 operator*(const Vec3& lhs, double rhs)
{
    return Vec3(lhs.e[0] * rhs,
        lhs.e[1] * rhs,
        lhs.e[2] * rhs);
}

Vec3 operator/(double t, const Vec3& rhs)
{
    return Vec3(t / rhs.e[0],
        t / rhs.e[1],
        t / rhs.e[2]);
}

Vec3 operator/(const Vec3& lhs,  double t)
{
    return Vec3(lhs.e[0] / t,
        lhs.e[1] / t,
        lhs.e[2] / t);
}

Vec3 Vec3::unit_vector(Vec3 v)
{
    return v / v.length();
}

/*
std::istream& operator>>(std::istream& is, Vec3& t)
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

std::ostream& operator<<(std::ostream& os, const Vec3& t)
{
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}
*/


void Vec3::make_unit_vector()
{
    double k = 1.0 / length();
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}

Vec3 Vec3::operator+(const Vec3& v2) const
{
    return Vec3(e[0] + v2.e[0],
        e[1] + v2.e[1],
        e[2] + v2.e[2]);
}

Vec3 Vec3::operator-(const Vec3& v2) const
{
    return Vec3(e[0] - v2.e[0],
        e[1] - v2.e[1],
        e[2] - v2.e[2]);
}

Vec3 Vec3::operator*(const Vec3& v2) const
{
    return Vec3(e[0] * v2.e[0],
        e[1] * v2.e[1],
        e[2] * v2.e[2]);
}

Vec3 Vec3::operator/(const Vec3& v2) const
{
    return Vec3(e[0] / v2.e[0],
        e[1] / v2.e[1],
        e[2] / v2.e[2]);
}

double Vec3::dot(const Vec3& v1, const Vec3& v2)
{
    return (v1.e[0] * v2.e[0]) + (v1.e[1] * v2.e[1]) + (v1.e[2] * v2.e[2]);
}

Vec3 Vec3::cross(const Vec3& v1, const Vec3& v2)
{
    return Vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
                (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
                (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

Vec3& Vec3::operator+=(const Vec3& v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];

    return *this;
}

Vec3& Vec3::operator-=(const Vec3& v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];

    return *this;
}

Vec3& Vec3::operator*=(const Vec3& v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];

    return *this;
}

Vec3& Vec3::operator/=(const Vec3& v)
{
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];

    return *this;
}

Vec3& Vec3::operator*=(const double t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;

    return *this;
}

Vec3& Vec3::operator/=(const double t)
{
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;

    return *this;
}

double Vec3::length() const
{
    double length = sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);

    return length;
}

std::string Vec3::toString()
{
    std::ostringstream ss;

    // Set precision
    ss << std::fixed << std::setprecision(2);

    ss << "X:" << e[0] << ", "
       << "Y:" << e[1] << ", "
       << "Z:" << e[2];

    return ss.str();
}