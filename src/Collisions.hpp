#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include "Scene.hpp"

inline bool will_camera_collide(scene *s, double x, double z) {
    for (size_t i = 0; i < s->boxes.size(); i++) {
        if (pow(s->boxes.at(i).xmin+1 - s->cam.x, 2) 
            + pow(s->boxes.at(i).ymin+1 - s->cam.y, 2) 
            + pow(s->boxes.at(i).zmin+1 - s->cam.z, 2) > 5) continue;
        if (!(
            s->cam.x + x < s->boxes.at(i).xmin - (x > 0 ? 0.2 : 0) ||
            s->cam.x + x > s->boxes.at(i).xmax + (x < 0 ? 0.2 : 0) ||
            s->cam.z + z < s->boxes.at(i).zmin - (z > 0 ? 0.2 : 0) ||
            s->cam.z + z > s->boxes.at(i).zmax + (z < 0 ? 0.2 : 0))) return true;

    }
    return false;
}

#endif