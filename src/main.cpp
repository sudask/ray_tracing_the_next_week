#include <iostream>
#include <random>
#include "float.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"

hitable *random_scene() {
    int n = 50000;
    hitable **list = new hitable*[n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for(int a = -10; a < 10; a++) {
        for(int b = -10; b < 10; b++) {
            float choose_mat = get_random_float();
            vec3 center(a + 0.9 * get_random_float(), 0.2, b + 0.9 * get_random_float());
            if((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if(choose_mat < 0.8) {
                    list[i++] = new moving_sphere(center, center + vec3(0, 0.5 * get_random_float(), 0), 0.0, 1.0, 0.2, new lambertian(vec3(get_random_float() * get_random_float(), get_random_float() * get_random_float(), get_random_float() * get_random_float())));
                }
                else if(choose_mat < 0.95) {
                    list[i++] = new sphere(center, 0.2, 
                            new metal(vec3(0.5 * (1 + get_random_float()), 0.5 * (1 + get_random_float()), 0.5 * (1 + get_random_float())), 0.5 * get_random_float()));
                }
                else {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list, i); 
}

vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    if(world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return vec3(0, 0, 0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    int nx = 600, ny = 300, ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    // hitable *list[5];
    // list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    // list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    // list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0));
    // list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    // list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    // hitable *world = new hitable_list(list, 5);

    // float R = cos(M_PI / 4);
    // hitable *list[2];
    // list[0] = new sphere(vec3(-R, 0, -1), R, new lambertian(vec3(0, 0, 1)));
    // list[1] = new sphere(vec3(R, 0, -1), R, new lambertian(vec3(1, 0, 0)));
    // hitable_list *world = new hitable_list(list, 2);

    // image on the cover of the book
    hitable *world = random_scene();

    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10;
    float aperture = 0.1;

    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);
    for(int j = ny - 1; j >= 0; j--) {
        for(int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for(int s = 0; s < ns; s++) {
                float u = float(i + get_random_float()) / float(nx);
                float v = float(j + get_random_float()) / float(ny);
                ray r = cam.get_ray(u, v);

                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }   
    }
}