#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <limits>
#include <iostream>
#include <cstdint>

#include "Vector.hpp"
#include "Camera.hpp"
#include "Box.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Render.hpp"

#define SPEED_MULT 4
#define ROT_MULT 4

unsigned int map[20][20] {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,0,1,1,1,1,1,1,1,2,1,0,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},
    {1,0,0,2,0,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
unsigned int spawn_x = 1, spawn_z = 1;

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

camera cam;
scene mainscene;

sf::Image canvas;
sf::Texture render_texture;
sf::RenderWindow window(sf::VideoMode(1000, 1000), "Render");

int main() {
    srand(time(NULL));
    cam.x = spawn_x*2+1; cam.y = 0, cam.z = spawn_z*2+1;
    cam.length = 0.1;
    cam.w = 0.2; cam.h = 0.2;
    cam.res_x = 64; cam.res_y = 64; // Preferably these two values will both be scaled versions of w and h, by a constant scalar
    cam.dir = normalise(make_vector(1, 0, 0)); // MUST be a unit vector

    canvas.create(cam.res_x, cam.res_y, sf::Color::White);
    mainscene.cam = cam;

    mainscene.boxes.push_back(make_box(0, -1, 0, 20*2+2, -3, 20*2+2, sf::Color(0x8c4313ff)));
    for (size_t y = 0; y < 20; y++) {
        for (size_t x = 0; x < 20; x++) {
            if (map[y][x] == 1) mainscene.boxes.push_back(make_box(x*2, -1, y*2, x*2+2, 1, y*2+2, sf::Color(0xa0a0a0ff)));
            if (map[y][x] == 2) mainscene.boxes.push_back(make_box(x*2, -1, y*2, x*2+2, 1, y*2+2, sf::Color(0x915b37ff)));
        }
    }

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
