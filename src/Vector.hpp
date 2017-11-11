#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <cmath>

struct vector {
    double x, y, z;
};
inline vector make_vector(double x, double y, double z) {
    vector tmp;
    tmp.x = x; tmp.y = y; tmp.z = z;
    return tmp;
}
inline vector vecmul(vector v, double scalar) {
    vector tmp = v;
    tmp.x *= scalar;
    tmp.y *= scalar;
    tmp.z *= scalar;
    return tmp;
}

inline void printvec(vector v) {
    std::cout << v.x << '\t'
        << v.y << '\t'
        << v.z << std::endl;
}

inline vector normalise(vector v) {
    double length = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
    v.x /= length;
    v.y /= length;
    v.z /= length;
    return v;
}

inline void normalise(double *x, double *y, double *z) {
    double length = sqrt(pow(*x, 2) + pow(*y, 2) + pow(*z, 2));
    *x /= length;
    *y /= length;
    *z /= length;
}

#endif