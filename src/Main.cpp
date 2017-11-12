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

#define SPEED_MULT 4
#define ROT_MULT 4

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
sf::Texture render_texture;
sf::RenderWindow window(sf::VideoMode(1000, 1000), "Render");

int main() {
    b  = make_box(-6, -1,  0, -4,  1,  2, sf::Color::Red);
    b2 = make_box( 1, -1, -8,  3,  1, -6, sf::Color::White);

    cam.x = 0; cam.y = 0, cam.z = -10;
    cam.length = 0.9;
    cam.w = 1; cam.h = 1;
    cam.res_x = 64; cam.res_y = 64; // Preferably these two values will both be scaled versions of w and h, by a constant scalar
    cam.dir = normalise(make_vector(0, 0, 1)); // MUST be a unit vector

    canvas.create(cam.res_x, cam.res_y, sf::Color::White);

    mainscene.cam = cam;
    mainscene.boxes = std::vector<box> {b, b2};

    render_texture.loadFromImage(canvas);

    sf::Clock delta_clock;
    while (window.isOpen()) {
        sf::Time delta = delta_clock.restart();
        window.setTitle("FPS: " + std::to_string(1/delta.asSeconds()));

        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            mainscene.cam.x += mainscene.cam.dir.x * SPEED_MULT * delta.asSeconds();
            mainscene.cam.z += mainscene.cam.dir.z * SPEED_MULT * delta.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            mainscene.cam.x += -mainscene.cam.dir.x * SPEED_MULT * delta.asSeconds();
            mainscene.cam.z += -mainscene.cam.dir.z * SPEED_MULT * delta.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            mainscene.cam.x += -mainscene.cam.dir.z * SPEED_MULT * delta.asSeconds();
            mainscene.cam.z +=  mainscene.cam.dir.x * SPEED_MULT * delta.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            mainscene.cam.x +=  mainscene.cam.dir.z * SPEED_MULT * delta.asSeconds();
            mainscene.cam.z += -mainscene.cam.dir.x * SPEED_MULT * delta.asSeconds();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
            mainscene.cam.dir = normalise(rot_vec_Y(mainscene.cam.dir, -ROT_MULT * delta.asSeconds()));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
            mainscene.cam.dir = normalise(rot_vec_Y(mainscene.cam.dir,  ROT_MULT * delta.asSeconds()));
    
        render(mainscene, &canvas);
        render_texture.update(canvas);
        sf::RectangleShape rect(sf::Vector2f(1000, 1000));
        rect.setPosition(sf::Vector2f(0, 0));
        rect.setTexture(&render_texture);

        window.clear();
        window.draw(rect);
        window.display();
    }
}
