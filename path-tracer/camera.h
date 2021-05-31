#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "utils.h"
#include "vec3.h"

//class camera {
//public:
//    vec3 o;
//    vec3 lower_left_corner;
//    vec3 horizontal;
//    vec3 vertical;
//    vec3 w, u, v;
//    double lens_radius;
//    double time0, time1;
//
//    camera() = default;
//    camera(vec3 lookf, vec3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_disk, double time0, double time1);
//    ray getray(double s, double t) const;
//
//    camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist, double t0 = 0, double t1 = 0):
//    o(lookfrom),
//    lens_radius(aperture / 2),
//    time0(t0),
//    time1(t1),
//    w(unit_vector(lookfrom - lookat)),
//    u(unit_vector(cross(vup, w))),
//    v(cross(w, u)) {
//        auto theta = degrees_to_radians(vfov);
//        auto half_height = tan(theta / 2);
//        auto half_width = aspect * half_height;
//
//        lower_left_corner = o
//            - half_width * focus_dist * u
//            - half_height * focus_dist * v
//            - focus_dist * w;
//
//        horizontal = 2 * half_width * focus_dist * u;
//        vertical = 2 * half_height * focus_dist * v;
//    }
//
//    ray get_ray(double s, double t) {
//        vec3 rd = lens_radius * random_unit_disk();
//        vec3 offset = u * rd.x() + v * rd.y();
//        return ray(
//            o+ offset,
//            lower_left_corner + s * horizontal + t * vertical - o - offset
//        );
//    }
//
//
//};


class cameranew {
public:
    vec3 o;
    vec3 lower_left_corner;
    vec3 hx;
    vec3 vy;
    vec3 w, u, v;
    double lens_radius;
    double time0, time1;

    cameranew() = default;
    cameranew(vec3 lookf, vec3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist, double t0, double t1);
    ray getray(double s, double t) const;

    ray getray(double s, double t) {
        vec3 rd = lens_radius * random_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(
            o + offset,
            lower_left_corner + s * hx + t * vy - o - offset);
    }
};

cameranew::cameranew(vec3 lookf, vec3 lookat, vec3 vup, double vfov,
    double aspect, double aperture, double focus_dist, double t0, double t1) :
    o(lookf), lens_radius(aperture * .5), time0(t0), time1(t1), w(unit_vector(lookf - lookat)) {
    double theta= degrees_to_radians(vfov);
    double half_height = tan(theta / 2);
    double half_width = aspect * half_height;
    vec3 u = (unit_vector(cross(vup, w)));
    vec3 v = cross(w,u);

    hx= 2 * half_width * focus_dist * u;
    vy= 2 * half_height * focus_dist * v;

    lower_left_corner = o - half_width * focus_dist * u- half_height * focus_dist * v - focus_dist * w;



}
#endif
