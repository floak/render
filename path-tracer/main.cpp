#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "object.h"
#include "utils.h"
#include "hittable_list.h"
#include "camera.h"
#include <random>
#include "material.h"
#include "texture.h"
#include "rectangle.h"
#include "cube.h"
#include "scene.h"

using namespace std;

vec3 ray_color(const ray& r, const hittable& world, int colorindex, int depth) {
    //int wave = colorindex + 380;
    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        vec3 attenuation;
        double pdf;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        vec3 temp;
        int num=6;
        if (depth > 1 && rec.mat_ptr->scatter(r, rec, attenuation, scattered, pdf, colorindex)) {
            //for (int wave = 380; wave < 782; wave++) {
            //    return emitted + attenuation * ray_color(scattered, world, wave, depth - 1); //* wavelengthtorgb(wave);////////
            //}
            //int wave = colorindex + 380;
            return  emitted  + attenuation * ray_color(scattered, world, colorindex, depth - 1);
        } else return emitted;

    } else return vec3(0.52, 0.80, 0.91);
    //else return vec3();
}

vec3 raycolor(const ray& r, const hittable& world, int depth,int colorindex) {
    hit_record rec;
    double pdf;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return vec3(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        vec3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered, pdf, colorindex)) {
            return attenuation * raycolor(scattered, world, depth - 1,colorindex);
        }
        return vec3(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.d());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

/////p(direction) = distance(p,q)^2 / ( cos(alpha) *A )????????problem?????????
    /*if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        double pdf; vec3 attenuation;
        if ( rec.mat_ptr->scatter(r, rec, attenuation, scattered, pdf)) {
            vec3 onlight = vec3(random_double(350 , 200.), 550,  random_double(340 ,220));
            vec3 tolight = onlight - rec.p;
            double distancesq = tolight.length_squared();
            tolight.norm();
            if (dot(tolight, rec.normal) < 0) return emitted;
            double lightarea = 150. * 120.; double lightcosine = fabs(tolight.y());
            if (lightcosine < 1e-6) return emitted;
            pdf = distancesq / (lightcosine * lightarea);
            scattered = ray(rec.p, tolight);

            return emitted + attenuation * rec.mat_ptr->scatterpdf(r, rec, scattered) * ray_color(scattered, background, world, depth + 1) / pdf;
        }
        else { return emitted; }
    }
    else return background;*/
    //if (depth <= 0)
    //    return vec3(0, 0, 0);

    //// If the ray hits nothing, return the background color.
    //if (!world.hit(r, 0.001, infinity, rec))
    //    return background;

    //ray scattered;
    //vec3 attenuation;
    //vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    //double pdf;
    //vec3 albedo;
    //if (!rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf))
    //    return emitted;
    //auto on_light = vec3(random_double(213, 343), 554, random_double(227, 332));
    //auto to_light = on_light - rec.p;
    //auto distance_squared = to_light.length_squared();
    //to_light = unit_vector(to_light);

    //if (dot(to_light, rec.normal) < 0)
    //    return emitted;

    //double light_area = (343 - 213) * (332 - 227);
    //auto light_cosine = fabs(to_light.y());
    //if (light_cosine < 0.000001)
    //    return emitted;

    //pdf = distance_squared / (light_cosine * light_area);
    //scattered = ray(rec.p, to_light);

    //return emitted
    //    + albedo * rec.mat_ptr->scatterpdf(r, rec, scattered)
    //    * ray_color(scattered, background, world, depth - 1) / pdf;

int main() {

    freopen("E:\\cornellbox\\mutlout64.ppm", "w", stdout);
    const vec3 background(0, 0, 0); 
    const int image_width = 400;
    const int image_height = 400;
    const auto aspect_ratio = image_width / image_height;
    const int samples_per_pixel = 1000;
    const int max_depth = 500;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    auto world = cornell_box();
    auto bal = ball();

    vec3 lookfrom(27.8, 27.8, -70.0);
    //vec3 lookfrom(27.8, 27.8, -120.0);
    vec3 lookat(27.8, 27.8, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    //auto vfov = 20.0;
    
    cameranew cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus,0.0,1.0);

    int nth = omp_get_num_procs();

#pragma omp parallel for num_threads(nth) schedule(dynamic,1)
    for (int j = 0; j <image_height; ++j) {
        std::cerr << "\rScanlines remaining:" << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            vec3 color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random()) / image_width;
                auto v = (j + random()) / image_height;
                ray r = cam.getray(u, v);
                /// <summary>

                //for (int wave = 380; wave < 781; wave++) {
                //    vec3 rays[] = { ray_color(r,world,wave,max_depth) };
                //    //color += ray_color(r, world, wave, max_depth);
                //    color += vec3((rays[wave]+rays[wave+1]+rays[wave+2])[0]/3, (rays[wave+1] + rays[wave + 2] + rays[wave + 3])[1] / 3, (rays[wave+2] + rays[wave + 3] + rays[wave + 4])[2] / 3);
                //}
                
                /*for (int wave = 0; wave < 401; wave++) {
                    vec3 rays[] = { ray_color(r,world,wave,max_depth) };
                    color += vec3((rays[wave] + rays[wave + 1] + rays[wave + 2])[0] / 3, (rays[wave + 1] + rays[wave + 2] + rays[wave + 3])[1] / 3, (rays[wave + 2] + rays[wave + 3] + rays[wave + 4])[2] / 3);
                }*/
                

                //vec3 rays[6] = { ray_color(r, world, 0, max_depth), ray_color(r, world, 1, max_depth), ray_color(r, world, 2, max_depth), ray_color(r, world, 3, max_depth), ray_color(r, world, 4, max_depth), ray_color(r, world, 5, max_depth) };
                vec3 rays[5] = { ray_color(r,world,0,max_depth),ray_color(r, world, 1, max_depth), ray_color(r, world, 2, max_depth), ray_color(r, world, 3, max_depth), ray_color(r, world, 4, max_depth) };
                //vec3 rays[16] = { ray_color(r, world, 0, max_depth), ray_color(r, world, 1, max_depth), 
                //    ray_color(r, world, 2, max_depth), ray_color(r, world, 3, max_depth),
                //    ray_color(r, world, 4, max_depth), ray_color(r, world, 5, max_depth),
                //ray_color(r, world, 6, max_depth), ray_color(r, world, 7, max_depth),
                //ray_color(r, world, 8, max_depth), ray_color(r, world, 9, max_depth) ,
                //ray_color(r, world, 10, max_depth), ray_color(r, world, 11, max_depth) ,
                //ray_color(r, world, 12, max_depth), ray_color(r, world, 13, max_depth) ,
                //ray_color(r, world, 14, max_depth), ray_color(r, world, 15, max_depth) };
                ////color += vec3((rays[0] + rays[1] + rays[2])[0] / 3, (rays[1] + rays[2] + rays[3])[1] / 3, (rays[3] + rays[4] + rays[5])[2] / 3);
                //color += vec3((rays[0]+rays[1]+rays[2])[0]/3,( rays[6]+rays[7] + rays[8])[1]/3, (rays[13] + rays[14] + rays[15])[2] / 3); 
                color += vec3((rays[0]+rays[1])[0]/2, (rays[2]+rays[3])[1]/2, (rays[3]+rays[4])[2]/2);
               
                //color += ray_color(r, bal,600, max_depth);

                //color += raycolor(r, bal,max_depth,1);
            }       
            color.write_color(std::cout, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}
