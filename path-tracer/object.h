#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "vec3.h"
#include "hittable.h"
#include "aabb.h"

class sphere : public hittable {
public:
	sphere() {}
	sphere(vec3 cen, double r,material* m) : center(cen), radius(r), mat_ptr(m) {};
	//virtual ~sphere(){ delete mat_ptr; }
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const;

public:
	vec3 center;
	double radius;
	material* mat_ptr;

};






//	Vector V = O - rayO;
//	double c = V.sqrlen() - radius * radius;
//	double b = -(V ^ rayD) * 2;
//	double a = rayD.sqrlen();
//	double delta = b * b - 4 * a * c;
//	if (delta > feps) {
//		double d1 = (-b - sqrt(delta)) / (2 * a);
//		double d2 = (-b + sqrt(delta)) / (2 * a);
//		collision.dist = d1;
//		if (d1 < 0 && d2 < 0) {
//			return false;
//		}
//		else if (d1 < 0 && d2 >= 0) {
//			collision.dist = d2;
//			collision.face = false;
//		}
//		else {
//			collision.dist = d1;
//			collision.face = true;
//		}
//		collision.belongs = this;
//		collision.C = rayO + rayD * collision.dist;
//		if (collision.face)
//			collision.N = (collision.C - O).unit();
//		else
//			collision.N = (O - collision.C).unit();
//		Vector NN = collision.N * (collision.N ^ rayD.reverse());
//		collision.I = rayD;
//		return true;
//	}
//	else {
//		return false;
//	}
//}
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.o() - center;
	auto a = r.d().length_squared();
	auto hb = dot(oc, r.d());
	auto c = oc.length_squared() - radius * radius;
	auto d = hb * hb - a * c;

	if (d > 0) {
		auto temp = (-hb - sqrt(d)) / a;
		if (temp< t_max && temp > t_min) {
			rec.t = temp; rec.p = r.at(rec.t); rec.normal = (rec.p - center) / radius;
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal); rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-hb + sqrt(d)) / a;
		if (temp< t_max && temp > t_min) {
			rec.t = temp; rec.p = r.at(rec.t); rec.normal = (rec.p - center) / radius;
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal); 
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

bool sphere::bounding_box(double t0, double t1, aabb& output_box) const {
	output_box = aabb(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}

#endif
