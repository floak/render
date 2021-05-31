#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "vec3.h"
#include "ray.h"
#include "texture.h"
#include "onb.h"
#include <vector>
#include <iostream>
using namespace std;

class material {
public:
	double m_refractindex[3];
	double GetRefractIndex(int index) const{ return m_refractindex[index]; }
	virtual vec3 emitted(double u, double v, const vec3 & p) const { return vec3(0, 0, 0); }
	//virtual vec3 emitted(int uu, int vv, const vec3& p) const { return vec3(0, 0, 0); }
	virtual double scatterpdf(const ray& r, const hit_record& rec, const ray& scattered) const { return 0; };
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf, int colorindex) const = 0;
};

class DIFF : public material {
public:
	texture* albedo;

	DIFF(const vec3& a) : albedo(new constant_texture(a)) {}
	DIFF(const int& a) : albedo(new constant_texture(a)) {}
	DIFF(texture* a) : albedo(a) {}
	virtual ~DIFF(){ delete albedo; }

	double scatterpdf(const ray& r, const hit_record& rec, const ray& scattered) const {
		double cosin = dot(rec.normal, scattered.d().retu());
		if (cosin < 0) return 0;
		return cosin / pi;
	}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf, int colorindex) const {
		onb uvw;
		uvw.build_from_w(rec.normal);
		do {
			vec3 dir = uvw.local(random_cosine_dir());
			scattered = ray(rec.p, dir.retu());
			pdf = dot(uvw.w(), scattered.d()) / pi;
		} while (pdf == double(0.));
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		//attenuation = albedo->wavevalue(rec.u, rec.v, rec.p);
		return true;
	}

	/*virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf, int colorindex) const {
		vec3 tar = rec.p + rec.normal + random_unit_sphere();
		scattered = ray(rec.p, tar - rec.p);
		attenuation = albedo->value(rec.u, rec.v, rec.p);
	}*/

};


class metal :public material {
public:
	vec3 albedo;
	double fuzz;
	metal(const vec3& a, double f = 0.) :albedo(a) { if (f < 1 && f >= 0)fuzz = f; else fuzz = 1; }

	virtual bool scatter(const ray&rin, const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf, int colorindex) const {
		vec3 reflected = reflect(unit_vector(rin.d()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.d(), rec.normal) > 0);
	}

	virtual double scatterpdf(const ray& r, const hit_record& rec, const ray& scattered) const {
		double cosin = dot(rec.p, scattered.d().retu());
		if (cosin < 0) cosin = 0;
		return cosin / pi;
	}
};






double schlick(double cosine, double ref_idx) {
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

double ntt(int wavelength) {
	double nt = ((wavelength - 380) / 1000) + 1.2;
	return nt;
}
class dielectricnew :public material {
public:
	constant_texture* albedo;
	//double nc = 1, nt[6] = { 1.5, 1.6, 1.7, 1.8, 1.9, 2.0 }; 
	double nc = 1, nt[6] = { 1.2,1.4, 1.6,1.8,2.0 };
	//double nc = 1, nt[16] = { 1.2,1.35,1.48,1.6,1.63,1.65,1.7,1.71,1.73,1.75,1.78,1.8,1.84,1.88,1.90,2.0};
	//double nc = 1,nt[20];///////////////
	//double nc = 1;
	
	//double nt= ((wavelength - 380) / 1000) + 1.2;

	
	virtual ~dielectricnew(){ delete albedo; }
	
	virtual bool scatter(const ray& rin, const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf, int colorindex) const {
		//
		attenuation = vec3(1, 1, 1);
		wavelengthtorgb(colorindex)[3];


		double etai_over_etat = (rec.front_face) ? (1 / nt[colorindex]) : (nt[colorindex] / 1);//折射率
		//double etai_over_etat = (rec.front_face) ? (1 / (wavelengthtorgb(600)[3])) : ((wavelengthtorgb(600)[3]) / 1);
		vec3 unit_direction = unit_vector(rin.d());
		double cos_theta = abs(dot(-unit_direction, rec.normal));
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		//
		if (etai_over_etat * sin_theta > 1.0) {
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}

		//
		double reflect_prob = schlick(cos_theta, etai_over_etat);
		if (random() < reflect_prob) {
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		} else {
			vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
			scattered = ray(rec.p, refracted);
			return true;
		}
	}
	
	virtual double scatterpdf(const ray& r, const hit_record& rec, const ray& scattered) const {
		double cosin = dot(rec.p, scattered.d().retu());
		if (cosin < 0) cosin = 0;
		return cosin / pi;
	}
};

//class glass :public material {
//public:
//	double refin;
//	vec3 color;
//	glass(double refin, const vec3& color = vec3(1,1,1));
//	virtual bool scatter(const ray& rin, const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf, int colorindex) const;
//};
//
//glass::glass(double refin,const vec3&color):refin(refin),color(color){}
//bool glass::scatter(const ray& rin, const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf, int colorindex) const {
//	vec3 reflected = reflect(rin.d(), rec.normal);
//	vec3
//};



class diffuse_light : public material {
public:
	diffuse_light(texture* a) : emit(a) {}
	diffuse_light(vec3 c) : emit(new constant_texture(c)) {}
	//diffuse_light(int c) : emit(new constant_texture(vec3(wavelengthtorgb(c)[0] / 255, wavelengthtorgb(c)[1] / 255, wavelengthtorgb(c)[2] / 255))) {}
	virtual ~diffuse_light(){ delete emit; }
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered,double&pdf, int colorindex) const {return false;}
	
	virtual vec3 emitted(double u, double v, const vec3& p) const {
		return emit->value(u, v, p);
		//return emit->wavevalue(u, v, p);
	}

	//virtual vec3 emitted(double u,  double v, const vec3& p) const {
	//	return emit->wavevalue(u, v, p);
	//	//return emit->wavevalue(u, v, p);
	//}

public:
	texture* emit;
};

#endif
