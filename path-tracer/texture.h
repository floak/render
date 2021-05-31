#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "vec3.h"
#include "ray.h"


class texture {
public:
	virtual vec3 value(double u, double v, const vec3& p) const = 0;
	virtual vec3 wavevalue(double u, double v, const vec3& p) const = 0;
};

class constant_texture : public texture {
public:
	constant_texture(){}
	constant_texture(vec3 c): color(c){}
	//constant_texture(int wave1, int wave2): bettercolor() {}
	constant_texture(int wave):RGB(vec3(wavelengthtorgb(wave)[0]/255, wavelengthtorgb(wave)[1]/255, wavelengthtorgb(wave)[2]/255)){}
	constant_texture(double red,double green,double blue):constant_texture(vec3(red,green,blue)){}
	virtual vec3 value(double u, double v, const vec3& p) const { return color; }

	//?scene?????int??vec3????????????
	virtual vec3 wavevalue(double u, double v, const vec3& p) const {
		//vec3 temp = vec3(wavelengthtorgb(uu)[0], wavelengthtorgb(uu)[1], wavelengthtorgb(uu)[2]);
		return RGB; }




private:
	vec3 color;
	vec3 RGB;
};

class checker_texture :public texture {
public:
	texture* odd;
	texture* even;
	checker_texture(){}
	checker_texture(texture* t0,texture* t1):even(t0),odd(t1){}
	checker_texture(vec3 c1, vec3 c2) : even(new constant_texture(c1)), odd(new constant_texture(c2)) {}
	virtual ~checker_texture(){ delete odd; delete even; }

	virtual vec3 value(double u, double v, const vec3& p) const {
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0) return odd->value(u, v, p);
		else return even->value(u, v, p);
	}
};

#endif
