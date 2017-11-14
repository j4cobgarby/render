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
#include "Collisions.hpp"

#define SPEED_MULT 3
#define ROT_MULT 2
#define MAP_WIDTH 20
#define MAP_HEIGHT 10

const char map_as_chars[MAP_HEIGHT][MAP_WIDTH+1] {
    "WWWWW###############",
    "W   W    #  #  #  ##",
    "W   W              #",
    "WW WWWW  #  #  #  ##",
    "W     W            #",
    "W     W  #  #  #  ##",
    "W     W            #",
    "W           #  #  ##",
    "W     W            #",
    "WWWWWWW#############",
};

unsigned int map[MAP_HEIGHT][MAP_WIDTH] {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,0,1,1,1,1,1,1,1,2,1,1,1,1,0,0,1},
    {1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,1,0,0,1},
    {1,0,0,2,0,1,0,1,0,0,0,1,1,1,0,0,1,0,0,1},
    {1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,0,1},
    {1,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,1,0,0,1},
    {1,0,1,1,1,1,0,1,0,1,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1},
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

    mainscene.boxes.push_back(make_box(0, -1, 0, (MAP_WIDTH-1)*2+2, -3, (MAP_HEIGHT-1)*2+2, sf::Color(0xaf3434ff)));
    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++) {
            if (map_as_chars[y][x] == '#') mainscene.boxes.push_back(make_box(x*2, -1, y*2, x*2+2, 1, y*2+2, sf::Color(0xa0a0a0ff)));
            if (map_as_chars[y][x] == 'W') mainscene.boxes.push_back(make_box(x*2, -1, y*2, x*2+2, 1, y*2+2, sf::Color(0x915b37ff)));
        }
    }

    render_texture.loadFromImage(canvas);

    sf::Clock delta_clock;
    sf::Clock global_clock;
    while (window.isOpen()) {
        sf::Time delta = delta_clock.restart();
        window.setTitle("FPS: " + std::to_string(1/delta.asSeconds()));

        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
        }

        double delta_x = 0, delta_z = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            delta_x += mainscene.cam.dir.x * SPEED_MULT * delta.asSeconds();
            delta_z += mainscene.cam.dir.z * SPEED_MULT * delta.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            delta_x -= mainscene.cam.dir.x * SPEED_MULT * delta.asSeconds();
            delta_z -= mainscene.cam.dir.z * SPEED_MULT * delta.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            delta_x -= mainscene.cam.dir.z * SPEED_MULT * delta.asSeconds();
            delta_z += mainscene.cam.dir.x * SPEED_MULT * delta.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            delta_x += mainscene.cam.dir.z * SPEED_MULT * delta.asSeconds();
            delta_z -= mainscene.cam.dir.x * SPEED_MULT * delta.asSeconds();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
            mainscene.cam.dir = normalise(rot_vec_Y(mainscene.cam.dir, -ROT_MULT * delta.asSeconds()));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
            mainscene.cam.dir = normalise(rot_vec_Y(mainscene.cam.dir,  ROT_MULT * delta.asSeconds()));

        if (!will_camera_collide(&mainscene, delta_x, 0)) mainscene.cam.x += delta_x;
        if (!will_camera_collide(&mainscene, 0, delta_z)) mainscene.cam.z += delta_z;
    
        render(mainscene, &canvas, global_clock.getElapsedTime());
        render_texture.update(canvas);
        sf::RectangleShape rect(sf::Vector2f(1000, 1000));
        rect.setPosition(sf::Vector2f(0, 0));
        rect.setTexture(&render_texture);

        window.clear();
        window.draw(rect);
        window.display();
    }
}
