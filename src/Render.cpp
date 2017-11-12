#include "Render.hpp"

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
    
    plane_topleft = make_vector(
        plane_left_dir.x * (s.cam.w / 2) + plane_middle.x,
        plane_middle.y + (s.cam.h / 2),
        plane_left_dir.z * (s.cam.w / 2) + plane_middle.z
    );

    ray r;
    r.x = s.cam.x;
    r.y = s.cam.y;
    r.z = s.cam.z;

    for (unsigned int y = 0; y < s.cam.res_y; y++) {
        for (unsigned int x = 0; x < s.cam.res_x; x++) {
            vector direction = make_vector(
                (plane_topleft.x + plane_right_dir.x * spacing_x * x) - s.cam.x,
                (plane_topleft.y + plane_down_dir.y * spacing_y * y) - s.cam.y,
                (plane_topleft.z + plane_right_dir.z * spacing_x * x) - s.cam.z
            );
            r.dir = normalise(direction);

            double dist = std::numeric_limits<double>::infinity();
            sf::Color col;
            bool hit = false;
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
                    min(col.r * (0.1/abs(dist)) * 10, 200.0),
                    min(col.g * (0.1/abs(dist)) * 10, 200.0), 
                    min(col.b * (0.1/abs(dist)) * 10, 200.0), 
                    255
                    )
                );
            else canvas->setPixel(x, y, sf::Color::Black);
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