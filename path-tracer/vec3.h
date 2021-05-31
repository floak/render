#ifndef __VEC3_H__
#define __VEC3_H__

#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include <cmath>

struct v3 {

};

class vec3 {
public:
	vec3() : e{ 0,0,0 } {}
	vec3(double e0, double e1, double e2) :e{ e0,e1,e2 } {}
	double x() const { return e[0]; } double y() const { return e[1]; } double z() const { return e[2]; }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }
	vec3 operator - () const { return vec3(-e[0], -e[1], -e[2]); }
	vec3 operator += (const vec3& a) { vec3(e[0] += a.e[0], e[1] += a.e[1], e[2] += a.e[2]); return *this; }
	vec3 operator*=(const double a) { vec3(e[0] *= a, e[1] *= a, e[2] *= a); return *this; }
	vec3 operator/=(const double a) { return *this *= 1 / a; }
	double length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	double length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	vec3 retu() const { return vec3(e[0] / length(), e[1] / length(), e[2] / length()); }
	vec3 norm() { return *this /= length(); }

	bool near_zero() const {
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

	void write_color(std::ostream& out,int samples_per_pixel) {
		auto scale = 1.0 / samples_per_pixel;  auto r = sqrt(scale * e[0]);auto g = sqrt(scale * e[1]);auto b = sqrt(scale * e[2]);
		out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
	}

	double e[3];
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];}
inline vec3 operator+(const vec3& u, const vec3& v) {return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);}
inline vec3 operator-(const vec3& u, const vec3& v) {return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);}
inline vec3 operator*(const vec3& u, const vec3& v) {return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);}
inline vec3 operator*(double t, const vec3& v) {return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);}
inline vec3 operator*(int t, const vec3& v) { return vec3(t * v.e[0], t * v.e[1], t * v.e[2]); }
inline vec3 operator+(int t, const vec3& v) { return vec3(t + v.e[0], t + v.e[1], t + v.e[2]); }
inline vec3 operator*(const vec3& v, double t) {return t * v;}
inline vec3 operator/(vec3 v, double t) {return (1 / t) * v;}

const double dot(const vec3 &u, const vec3 &v) { return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]; }
inline vec3 cross(const vec3& u, const vec3& v) { return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]); }
inline vec3 unit_vector(vec3 v) {return v / v.length();}

vec3 reflect(const vec3& v, const vec3& n) { return v - 2 * dot(v, n) * n; }

const double random(){
	static std::mt19937 mt;
	static std::uniform_real_distribution<double> rtrand;
	return rtrand(mt);
}

const double random_double(double min, double max) { return min + (max - min) * random(); }

const int random_int(int min, int max) {
	return static_cast<int>(random_double(min, max + 1));
}

const vec3 random_unit_sphere() {
	vec3 p;
	do {
		p = 2.0 * vec3(random(), random(), random()) - vec3(1, 1, 1);
	} while (dot(p, p) >= 1.0);
	return p;
}

const vec3 random_unit_vector() {
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return vec3(r * cos(a), r * sin(a), z);
}

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	//vec3 atten;
	auto cos_theta = dot(-uv, n);

	vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n;
	//atten = vec3(0.65, 0.05, 0.05);
	
	return r_out_parallel + r_out_perp;
}

//单位圆盘随机点函数
const vec3 random_unit_disk() {
	vec3 p; do { p = 2.0 * vec3(random(), random(), 0) - vec3(1, 1, 0); } while (dot(p, p) >= 1.0);
	return p;
}

const vec3 random_cosine_dir() {
	double r1 = random(), r2 = random(), z = sqrt(1 - r2), phi = 2 * pi * r1,
		x = cos(phi) * 2 * sqrt(r2), y = sin(phi) * 2 * sqrt(r2);
	return vec3(x, y, z);
}



#endif
