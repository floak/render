#pragma once
#include "vec3.h"
#include "utils.h"
#include "rectangle.h"
#include "cube.h"
#include "material.h"
#include "object.h"

hittable_list cornell_box() {

    hittable_list objects;

    auto red = new DIFF(new constant_texture(vec3(0.65, 0.05, 0.05)));
    auto blue = new DIFF(new constant_texture(vec3(0.05, 0.05, 0.73)));
    auto white = new DIFF(new constant_texture(vec3(0.88, 0.88, 0.88)));
    auto green = new DIFF(new constant_texture(vec3(0.12, 0.45, 0.15)));
    auto light = new diffuse_light(new constant_texture(vec3(20, 20, 20)));
    //auto red = new lambertian(new constant_texture(400));


    /*auto red = new lambertian(new constant_texture(400));
    auto blue = new lambertian(new constant_texture(500));
    auto white = new lambertian(new constant_texture(600));
    auto green = new lambertian(new constant_texture(700));
    auto light = new diffuse_light(new constant_texture(vec3(20, 20, 20)));*/
    auto glass = new dielectricnew();
    auto mirror = new metal(vec3(0.8, 0.8, 0.8), 0.);

    objects.add(new yz_rect(0, 55.5, 0, 55.5, 55.5, green));
    objects.add(new yz_rect(0, 55.5, 0, 55.5, 0, red));
    objects.add(new xz_rect(20.3, 35.3, 21.7, 34.2, 55.0, light));
    objects.add(new xz_rect(0, 55.5, 0, 55.5, 0, white));
    objects.add(new xy_rect(0, 55.5, 0, 55.5, 55.5, white));
    objects.add(new xz_rect(0, 55.5, 0, 55.5, 55.5, white));
    hittable* box1 = new box(vec3(0, 0, 0), vec3(16.5, 33.0, 16.5), mirror);
    box1 = new rotate_y(box1, 25);
    box1 = new translate(box1, vec3(26.5, 0, 29.5));
    objects.add(box1);
    objects.add(new sphere(vec3(18.0, 8.0, 20.0), 8.0, glass));

    return objects;

}

hittable_list ball() {
    hittable_list objects;

    auto mirror = new metal(vec3(0.8, 0.8, 0.8), 0.);
    auto green = new DIFF(new constant_texture(vec3(0.12, 0.45, 0.15)));
    objects.add(new sphere(vec3(18.0, 9.0, 40), 8.0, green));
    objects.add(new sphere(vec3(0, -1000, 0), 1000, mirror));
    objects.add(new sphere(vec3(38.0, 18.0, 50.0), 18.0, mirror));

    return objects;
}

hittable_list earth() {
    int nx, ny, nn;
    unsigned char* texture_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);

    /*auto earth_surface =
        make_shared<lambertian>(new constant(texture_data, nx, ny));
    auto globe = make_shared<sphere>(vec3(0, 0, 0), 2, earth_surface);*/

    return hittable_list(globe);
}