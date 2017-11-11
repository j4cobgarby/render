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

box b, b2;
camera cam;
scene mainscene;

sf::Image canvas;
sf::RenderWindow window(sf::VideoMode(1000, 1000), "Render");

int main() {
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
    cam.res_x = 64; cam.res_y = 64; // Preferably these two values will both be scaled versions of w and h, by a constant scalar
    cam.dir = normalise(make_vector(0, 0, 1)); // MUST be a unit vector

    canvas.create(cam.res_x, cam.res_y, sf::Color::Black);
    
    mainscene.cam = cam;
    mainscene.boxes = std::vector<box> {
        b, b2
    };

    render(mainscene, &canvas);

    canvas.saveToFile("../out.png");

    sf::Clock delta_clock;
    while (window.isOpen()) {
        sf::Time delta = delta_clock.restart();

        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mainscene.cam.z += 2 * delta.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mainscene.cam.z -= 2 * delta.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mainscene.cam.x -= 2 * delta.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mainscene.cam.x += 2 * delta.asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
            mainscene.cam.dir = normalise(rot_vec_Y(mainscene.cam.dir, -4 * delta.asSeconds()));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
            mainscene.cam.dir = normalise(rot_vec_Y(mainscene.cam.dir,  4 * delta.asSeconds()));
    
        render(mainscene, &canvas);
        sf::Texture tex;
        tex.loadFromImage(canvas);
        sf::RectangleShape rect(sf::Vector2f(1000, 1000));
        rect.setPosition(sf::Vector2f(0, 0));
        rect.setTexture(&tex);

        window.clear();
        window.draw(rect);
        window.display();
    }
}
