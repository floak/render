#ifndef __OBJ_H__
#define __OBJ_H__ 

#include "utils.hpp"
#include "ray.hpp"
//#include "bezier.hpp"

// enum Refl_t { DIFF, SPEC, REFR };

unsigned short mess[3]={1, 2, 3};

class Texture {
public:
	vec3 color;
	vec3 emitted;
	Refl_t refl;
	double etai;

	int *tmpp;
	int w, h, c;
	Texture(const Texture&t):  color(t.color), emitted(t.emitted), etai(t.etai),refl(t.refl) {}
	Texture(double b, vec3 col, vec3 e, Refl_t r): etai(b), color(col), emitted(e), refl(r) {}



	Refl_t getreflt(double u,double v){
		if(tmpp==NULL) return refl;
		int tmpw = u*w;
		int tmph=v*h;
		int iw= (tmpw%w+w)%w;
		int ih= (tmph%h+h)%h;

		int x = tmpp[ih*w*c+iw*c];
		int y = tmpp[ih*w*c+iw*c+1];
		int z = tmpp[ih*w*c+iw*c+2];

		if(x== y == z ==233){
			return SPEC;
		}
		return refl;

	}

	vec3 getcolor(double u,double v){
		if(tmpp==NULL) return color;
		int tmpw = u*w;
		int tmph=v*h;
		int iw= (tmpw%w+w)%w;
		int ih= (tmph%h+h)%h;

		int x = tmpp[ih*w*c+iw*c];
		int y = tmpp[ih*w*c+iw*c+1];
		int z = tmpp[ih*w*c+iw*c+2];

		if(x== y == z ==233){
			return vec3(1, 1, 1);
		}
		return vec3(x, y, z) / 255;

	}
};

class Object {
public:
	Texture texture;
	Object(Texture t): texture(t) {}
	Object(Refl_t refl, vec3 color, vec3 emitted, double etai):
		texture(etai, color, emitted, refl) {}

	virtual pair<double, vec3> hit(Ray) {}
	virtual double hitnum(Ray){}
	virtual vec3 hitray(Ray){}

		// If no hit, then return (INF, (0,0,0))
	//virtual std::pair<vec3, vec3> aabb() {}
	virtual vec3 norm(vec3) {}
		// return norm vec out of obj
};







class SphereObject: public Object {
public:
	vec3 o; 
	double r;

	SphereObject(vec3 o_, double r_, Texture t):
		o(o_), r(r_), Object(t) {}
	SphereObject(vec3 o_, double r_, Refl_t refl, double etai = 1.5, vec3 color = vec3(), vec3 emitted = vec3()):
		o(o_), r(r_), Object(refl, color, emitted, etai) {}



	virtual pair<double, vec3> hit(Ray ray) {
		vec3 ro = o - ray.o;
		double b = ray.d.dot(ro);
		double d = sqr(b) - ro.dot(ro) + sqr(r);

		if (d < 0) return std::make_pair(INF, vec3());
		else d = sqrt(d);
		
		double t=b-d;
	    if (b-d>1e-6){t=b-d;}
	    else{
	        t=b+d;
		if(b+d>1e-6){t=b+d;}
	 	else t=-1;}

		if (t < 0)
			return std::make_pair(INF, vec3());
		return std::make_pair(t, ray.get(t));
	}

	
	virtual vec3 norm(vec3 p) {
		double d = std::abs((p - o).len() - r);
		assert(d < 1e-6);
		return (p - o).norm();
	}


	virtual double hitnum(Ray ray){
		vec3 ro =o-ray.o;
		double b=ray.d.dot(ro);
		auto c = ro.dot(ro)-r*r;
		double d =b*b- c;

		if(d<0){
			return INF;
		}

		if(d>=0){
			d=sqrt(d);
		}
		double t=b-d;
		if (b-d>1e-6){t=b-d;}
		else{
			t=b+d;
			if(b+d>1e-6){t=b+d;}
			else t=-1;
		}
		if(t<0){
			return INF;
		}

		return t;
		
	}

	virtual vec3 hitt(Ray ray){
		vec3 ro =o-ray.o;
		double b=ray.d.dot(ro);
		auto c = ro.dot(ro)-r*r;
		double d =b*b- c;

		if(d<0){
			return vec3(0,0,0);
		}

		if(d>=0){
			d=sqrt(d);
		}
		double t=b-d;
		if (b-d>1e-6){t=b-d;}
		else{
			t=b+d;
			if(b+d>1e-6){t=b+d;}
			else t=-1;
		}
		if(t<0){
			return vec3(0,0,0);
		}

		return ray.get(t);
	 
    }
  };








#endif // __OBJ_H__
