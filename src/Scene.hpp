#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "Camera.hpp"
#include "Box.hpp"

struct scene {
    camera cam;
    std::vector<box> boxes;
};

#endif