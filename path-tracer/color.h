#pragma once
#include "vec3.h"
#include<iostream>


class color {
public:
	int elem[3];
	int &r = elem[0],  &g = elem[1],  &b = elem[2];
	color(int r, int g, int b) :elem{ r,g,b } {};
	color(int u = 0);
	color(const vec3& v);
	color& operator=(const vec3& v);
	int operator[](int index) const { return elem[index]; };
	int& operator[](int index) { return elem[index]; };

	color (int u):elem{u,u,u}{}

	//color(const vec3& v) { *this = v; }
};

color::color(const vec3& v) { *this = v; }

color &color::operator=(const vec3& v) {
	vec3 n = v;
	if (n.x() > 1) { n.x() == 1; }
	if (n.y() > 1) { n.y() == 1; }
	if (n.x() > 1) { n.x() == 1; }
	n = vec3{ sqrt(n.x()),sqrt(n.y()),sqrt(n.z()) };

	n *= 255.999;
	r = static_cast<int>(n.x()), g = static_cast<int>(n.y()), b= static_cast<int>(n.z());
	return *this;

}

void write_color(std::ostream& out, color samples_per_pixel) {
	auto scale = 1.0 / samples_per_pixel;  auto r = sqrt(scale * e[0]); auto g = sqrt(scale * e[1]); auto b = sqrt(scale * e[2]);
	out << static_cast<int>(255.999 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << '\n';
}

