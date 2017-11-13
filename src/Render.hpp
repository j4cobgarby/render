#ifndef RENDER_HPP
#define RENDER_HPP

#include <cmath>
#include <SFML/Graphics.hpp>

#include "Vector.hpp"
#include "Scene.hpp"
#include "Ray.hpp"

using std::min;
using std::max;

bool raytest(ray, box, double*);
void render(scene, sf::Image*, sf::Time);

#endif