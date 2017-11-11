#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Vector.hpp"

struct camera {
    double x, y, z; // Origin
    double length; // Focal length
    double w, h; // Physical size of 'sensor'
    unsigned int res_x, res_y; // Resolution scalar
    vector dir; // Camera can't look up
};

#endif