#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <limits>
#include <iostream>

#include "Vector.hpp"
#include "Camera.hpp"
#include "Box.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Render.hpp"

/*

+Y

 |
 |     +Z
 |    /
 |   /
 |  /
 | /
 O -- -- -- +X

*/

int main() {
    box b, b2;
    camera cam;

    b.xmin = -6; b.xmax = -4;
    b.ymin = -1; b.ymax = 1;
    b.zmin = 0; b.zmax = 2;
    b.colour = sf::Color::Red;

    b2.xmin = 1; b2.xmax = 3;
    b2.ymin = -1; b2.ymax = 1;
    b2.zmin = -8; b2.zmax = -6;
    b2.colour = sf::Color::Green;

    cam.x = 0; cam.y = 0, cam.z = -10;
    cam.length = 0.7;
    cam.w = 1; cam.h = 1;
    cam.res_x = 128; cam.res_y = 128; // Preferably these two values will both be scaled versions of w and h, by a constant scalar
    cam.dir = normalise(make_vector(0, 0, 1)); // MUST be a unit vector

    sf::Image canvas;
    canvas.create(cam.res_x, cam.res_y, sf::Color::Black);
    
    scene mainscene;
    mainscene.cam = cam;
    mainscene.boxes = std::vector<box> {
        b, b2
    };

    render(mainscene, &canvas);

    canvas.saveToFile("../out.png");
}
