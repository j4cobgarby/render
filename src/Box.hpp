#ifndef BOX_HPP
#define BOX_HPP

#include <SFML/Graphics.hpp>

struct box {
    double xmin, ymin, zmin,
            xmax, ymax, zmax;
    sf::Color colour; 
};

inline box make_box(double xmin, double ymin, double zmin,
                    double xmax, double ymax, double zmax,
                    sf::Color colour) {
    box tmp;
    tmp.xmin = xmin; tmp.ymin = ymin; tmp.zmin = zmin;
    tmp.xmax = xmax; tmp.ymax = ymax; tmp.zmax = zmax;
    tmp.colour = colour;
    return tmp;
}

#endif