#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "utils.h"
#include "vec3.h"
#include "ray.h"
#include "hittable.h"

class xy_rect : public hittable {
public:
    xy_rect() {}
    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, material* mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
    virtual ~xy_rect(){ delete mp; }
    virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const;

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const {
        output_box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
        return true;
    }

public:
    material* mp;
    double x0, x1, y0, y1, k;
};

bool xy_rect::hit(const ray& r, double t0, double t1, hit_record& rec) const {

    auto t = (k - r.o().z()) / r.d().z();
    if (t < t0 || t > t1) return false;
    auto x = r.o().x() + t * r.d().x();
    auto y = r.o().y() + t * r.d().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    vec3 outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

class xz_rect : public hittable {
public:
    xz_rect() {}
    xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, material* mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
    virtual ~xz_rect(){ delete mp; }
    virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const;

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const {
        output_box = aabb(vec3(x0, k - 0.0001, z0), vec3(x1, k + 0.0001, z1));
        return true;
    }

public:
    material* mp;
    double x0, x1, z0, z1, k;
};

bool xz_rect::hit(const ray& r, double t0, double t1, hit_record& rec) const {
    auto t = (k - r.o().y()) / r.d().y();
    if (t < t0 || t > t1)
        return false;
    auto x = r.o().x() + t * r.d().x();
    auto z = r.o().z() + t * r.d().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    vec3 outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

class yz_rect : public hittable {
public:
    yz_rect() {}
    yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, material* mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
    virtual ~yz_rect(){ delete mp; }
    virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const;

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const {
        output_box = aabb(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
        return true;
    }

public:
    material* mp;
    double y0, y1, z0, z1, k;
};

bool yz_rect::hit(const ray& r, double t0, double t1, hit_record& rec) const {
    auto t = (k - r.o().x()) / r.d().x();
    if (t < t0 || t > t1)
        return false;
    auto y = r.o().y() + t * r.d().y();
    auto z = r.o().z() + t * r.d().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    vec3 outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

#endif
