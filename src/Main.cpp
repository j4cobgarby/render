#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <limits>
#include <iostream>

using std::max;
using std::min;

/**
 * A 3-dimensional vector
 */
struct vector {
    double x, y, z;
};
vector make_vector(double x, double y, double z) {
    vector tmp;
    tmp.x = x; tmp.y = y; tmp.z = z;
    return tmp;
}
vector vecmul(vector v, double scalar) {
    vector tmp = v;
    tmp.x *= scalar;
    tmp.y *= scalar;
    tmp.z *= scalar;
    return tmp;
}

void printvec(vector v) {
    std::cout << v.x << '\t'
        << v.y << '\t'
        << v.z << std::endl;
}

vector normalise(vector v) {
    double length = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
    v.x /= length;
    v.y /= length;
    v.z /= length;
    return v;
}

void normalise(double *x, double *y, double *z) {
    double length = sqrt(pow(*x, 2) + pow(*y, 2) + pow(*z, 2));
    *x /= length;
    *y /= length;
    *z /= length;
}

struct camera {
    double x, y, z; // Origin
    double length; // Focal length
    double w, h; // Physical size of 'sensor'
    unsigned int res_x, res_y; // Resolution scalar
    vector dir; // Camera can't look up
};

struct box {
    double xmin, ymin, zmin,
            xmax, ymax, zmax;
    sf::Color colour; 
};

struct ray {
    double x, y, z; // Origin
    vector dir;
};

struct scene {
    camera cam;
    std::vector<box> boxes;
};

bool raytest(ray, box, double*);
void render(scene, sf::Image*);

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

    b.xmin = -1; b.xmax = 1;
    b.ymin = -1; b.ymax = 1;
    b.zmin = 0; b.zmax = 2;
    b.colour = sf::Color::Red;

    b2.xmin = 2; b2.xmax = 4;
    b2.ymin = 0.5; b2.ymax = 2.5;
    b2.zmin = -3; b2.zmax = -1;
    b2.colour = sf::Color::Green;

    cam.x = 5; cam.y = 0, cam.z = -10;
    cam.length = 0.9;
    cam.w = 1; cam.h = 1;
    cam.res_x = 100; cam.res_y = 100; // Preferably these two values will both be scaled versions of w and h, by a constant scalar
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

void render(scene s, sf::Image *canvas) {
    double spacing_x = fabs(s.cam.w / (s.cam.res_x - 1));
    double spacing_y = fabs(s.cam.h / (s.cam.res_y - 1));

    vector plane_middle; // The center of the s.camera plane.
    vector plane_topleft; // The position of the top-left-hand corner of the s.camera plane. The ray for the first pixel goes through this point
    vector plane_left_dir; // The (normalised) direction left along the plane
    vector plane_down_dir; // The (normalised) direction down along the plane
    vector plane_right_dir;

    plane_middle = make_vector(s.cam.x + s.cam.dir.x*s.cam.length, 
        s.cam.y + s.cam.dir.y*s.cam.length, 
        s.cam.z + s.cam.dir.z*s.cam.length);
    plane_left_dir = make_vector(-s.cam.dir.z, 0, s.cam.dir.x);
    plane_down_dir = make_vector(0, -1, 0); // it'll always be this
    plane_right_dir = make_vector(s.cam.dir.z, 0, -s.cam.dir.x);
    //plane_left_dir = plane_right_dir;
    // so..
    // topleft =
    // x = left_dir.x * (plane width / 2) + plane_middle.x
    // y = plane_middle.y + plane height / 2
    // z = left_dir.z * (plane width / 2) + plane_middle.z
    plane_topleft = make_vector(
        plane_left_dir.x * (s.cam.w / 2) + plane_middle.x,
        plane_middle.y + (s.cam.h / 2),
        plane_left_dir.z * (s.cam.w / 2) + plane_middle.z
    );

    for (unsigned int y = 0; y < s.cam.res_y; y++) {
        for (unsigned int x = 0; x < s.cam.res_x; x++) {
            vector direction = make_vector(
                (plane_topleft.x + plane_right_dir.x * spacing_x * x) - s.cam.x,
                (plane_topleft.y + plane_down_dir.y * spacing_y * y) - s.cam.y,
                (plane_topleft.z + plane_right_dir.z * spacing_x * x) - s.cam.z
            );
            ray r;
            r.x = s.cam.x;
            r.y = s.cam.y;
            r.z = s.cam.z;
            r.dir = normalise(direction);

            double dist = std::numeric_limits<double>::infinity();
            sf::Color col;
            bool hit;
            for (size_t i = 0; i < s.boxes.size(); i++) {
                double tmp_dist;
                if (raytest(r, s.boxes.at(i), &tmp_dist)) {
                    if (tmp_dist < dist) {
                        col = s.boxes.at(i).colour;
                        dist = tmp_dist;
                    }
                    hit = true;
                }
            }
            if (hit) canvas->setPixel(x, y, 
            sf::Color(
                col.r * (10/(dist*2)), 
                col.g * (10/(dist*2)), 
                col.b * (10/(dist*2)), 
                col.a
                )
            );
        }
    }
}

bool raytest(ray r, box b, double *distance) {
    vector inv_dir;
    inv_dir.x = 1 / r.dir.x;
    inv_dir.y = 1 / r.dir.y;
    inv_dir.z = 1 / r.dir.z;

    double tmin = -std::numeric_limits<double>::infinity();
    double tmax =  std::numeric_limits<double>::infinity();

    double t1 = (b.xmin - r.x) * inv_dir.x;
    double t2 = (b.xmax - r.x) * inv_dir.x;
    tmin = max(tmin, min(min(t1, t2), tmax));
    tmax = min(tmax, max(max(t1, t2), tmin));

    t1 = (b.ymin - r.y) * inv_dir.y;
    t2 = (b.ymax - r.y) * inv_dir.y;
    tmin = max(tmin, min(min(t1, t2), tmax));
    tmax = min(tmax, max(max(t1, t2), tmin));

    t1 = (b.zmin - r.z) * inv_dir.z;
    t2 = (b.zmax - r.z) * inv_dir.z;
    tmin = max(tmin, min(min(t1, t2), tmax));
    tmax = min(tmax, max(max(t1, t2), tmin));

    *distance = tmin;
    return tmax > max(tmin, 0.0);
}
