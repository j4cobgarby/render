#ifndef BOX_HPP
#define BOX_HPP

#include <SFML/Graphics.hpp>

struct box {
    double xmin, ymin, zmin,
            xmax, ymax, zmax;
    sf::Color colour; 
};

#endif