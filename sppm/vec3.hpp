#ifndef __VEC3_H__
#define __VEC3_H__ 

#include "utils.hpp"

class vec3{
public:
	double x;
	double y;
	double z;

	vec3(double x_=0, double y_=0, double z_=0): x(x_), y(y_), z(z_) {}

	vec3 operator-() const {
		return vec3(-x, -y, -z);
		}

	vec3 operator+(const vec3&u) const {
		return vec3(x+u.x, y+u.y, z+u.z);
		}

	vec3 operator-(const vec3&u) const {
		return vec3(x-u.x, y-u.y, z-u.z);
		}

	vec3 operator+(double p) const {
		return vec3(x+p, y+p, z+p);
		}

	vec3 operator-(double p) const {
		return vec3(x-p, y-p, z-p);
		}
	vec3 operator*(double p) const {
		return vec3(x*p, y*p, z*p);
		}
	vec3 operator/(double p) const {
		return vec3(x/p, y/p, z/p);
		}

	bool operator==(const vec3&u) const {
		return x==u.x && y==u.y && z==u.z;
		}
	bool operator!=(const vec3&u) const {
		return x!=u.x || y!=u.y || z!=u.z;
		}
	vec3&operator+=(const vec3&u) {
		return *this = *this + u;
		}
	vec3&operator-=(const vec3&u) {
		return *this = *this - u;
		}
	vec3&operator+=(double p) {
		return *this = *this + p;
		}
	vec3&operator-=(double p) {
		return *this = *this - p;
		}
	vec3&operator*=(double p) {
		return *this = *this * p;}
	vec3&operator/=(double p) {return *this = *this / p;
	}

	double operator|(const vec3&u) const {return x*u.x + y*u.y + z*u.z;}
	double dot(const vec3&u) const {return x*u.x + y*u.y + z*u.z;}
	double max() const {return x>y&&x>z?x:y>z?y:z;}
	vec3 max(const vec3&u) const {return vec3(std::max(x,u.x), std::max(y,u.y), std::max(z,u.z));}
	vec3 min(const vec3&u) const {return vec3(std::min(x,u.x), std::min(y,u.y), std::min(z,u.z));}
	double len() const {return sqrt(x*x + y*y + z*z);}
	double len2() const {return x*x + y*y + z*z;}

	
	vec3 com(const vec3&u) const {return vec3(x*u.x, y*u.y, z*u.z);}
	vec3 operator&(const vec3&u) const {return vec3(y*u.z-z*u.y, z*u.x-x*u.z, x*u.y-y*u.x);}
	vec3 cross(const vec3&u) const {return vec3(y*u.z-z*u.y, z*u.x-x*u.z, x*u.y-y*u.x);}
	vec3 norm() const {return (*this)/len();}
	//vec3 clip(double r0=0, double r1=1) const {return vec3(x>r1?r1:x<r0?r0:x, y>r1?r1:y<r0?r0:y, z>r1?r1:z<r0?r0:z);}

	vec3 reflect(const vec3&n) const {return (*this)-n*2.*n.dot(*this);}

	inline double randomm(){ return (double)rand()/ (double)RAND_MAX;}
	inline vec3 operator*(){}


	
	vec3 refract(const vec3&n, double nt, double nc) const { 
		double cosi = norm().dot(n);
		//double nir = ni / nr;
		double cosr2 = 1 - (nt/nc)*(nt/nc)*(1-cosi*cosi);
		if (cosr2 <= 0)
			return vec3();
		double cosr = sqrt(cosr2);
		if (cosi > 0) // out
			cosr = -cosr;
		return ((*this)*(nt/nc) - n*((nt/nc)*cosi+cosr)).norm();
		//return *this;
	}
	


	vec3 refractt(const vec3&n, double etai_etat){
		double costheta = norm().dot(n);
		vec3 r_out_parall=n*etai_etat*costheta;
		vec3 r_out_prep = n*(-sqrt(1-r_out_parall.len2()));
		return r_out_parall+r_out_prep;
	}
};

inline double randomm(){ return (double)rand()/ (double)RAND_MAX;}

vec3 min(vec3 u, vec3 b) {return vec3(std::min(u.x, b.x), std::min(u.y, b.y), std::min(u.z, b.z));}
vec3 max(vec3 u, vec3 b) {return vec3(std::max(u.x, b.x), std::max(u.y, b.y), std::max(u.z, b.z));}

struct vec2 {
	double n;
	vec3 f;
	vec2(): n(0), f(0,0,0) {}

	vec2(double n_, vec3 f_): n(n_), f(f_) {}

	void add(vec3 c, double p = 1.) { n += p, f += c; }

	//vec3 getcol() { return f / n; }

	void reset() { n = 0; f.x = f.y = f.z = 0; }

	vec2 operator+(const vec2& u) const {
		return vec2(n + u.n, f + u.f);
		}

	vec2 operator*(double p) const {
		return vec2(n * p, f * p);
		}
	vec2 operator/(double p) const {
		return vec2(n / p, f / p);
		}
	vec2& operator+=(const vec2& u) {
		return *this = *this + u;
		}

	vec3 get() { 
		if(n<1e-6){
			return f;
		}
		else{
			return f/n;
		}

		//return n < eps ? f : f / n; 
	}

};


struct noderec {
	vec3 pos;
	vec3 col;
	vec3 dir;
	int index;
	double prob, r;

	
	noderec() {index = -1; prob = 1;}

	noderec(vec3 pos0, vec3 col0, vec3 dir0, double r0 = 1, int index0 = -1, double prob0 = 1):
		pos(pos0), col(col0), dir(dir0), index(index0), prob(prob0), r(r0) {}
};

// assume vec3() == NULL (0,0,0)


// class vec3{
// public:
// double e[3]
// vec3():e{0,0,0}{}
// vec3(double e0,double e1,double e2):e{e0,e1,e2}{}
// double x const {return e[0];}
// double y const {return e[1];}
// double z const {return e[2];}
// vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
// vec3 operator+(const vec3&u) {vec3+

// };

#endif // __VEC3_H__