#ifndef __RAY_H__
#define __RAY_H__ 

#include "utils.hpp"
#include "vec3.hpp"

class Ray
{
public:
	vec3 o;
	vec3 d;

	Ray(vec3 o0, vec3 d0): o(o0), d(d0) {}

	vec3 get(double t) { 
		return o + d * t; 
		}
	
};

#endif // __RAY_H__