#ifndef __RENDER_H__
#define __RENDER_H__ 

//#include "texture.hpp"
#include "obj.hpp"
#include "scene.hpp"
#include "kdtree.hpp"
using namespace std;




// pair<int, double> hitrecord(Ray ray) {
// 	double t = INF;
// 	int id = -1;
// 	for (int i = 0; i < scene_num; ++i) {
// 		std::pair<double, vec3> tmp = scene[i]->hit(ray);

// 		double hittmp1=scene[i]->hitnum(ray);
// 		vec3 hittmp2=scene[i]->hitray(ray);



// 		if (hittmp1 < INF / 2 && hittmp2.len2() > 1e-6 && hittmp1 < t) {
// 			t = hittmp1;
// 			id = i;
// 		}
// 	}
// 	return make_pair(id, t);
// }


// double hitidx(Ray r){
// 	double t=infinity;
// 	int fi=-1;
// 	for (int i = 0; i < scene_num; ++i){
// 		double hittmp1=scene[i]->hitnum(r);
// 		vec3 hittmp2=scene[i]->hitray(r);
// 		if(hittmp1<500000&&hittmp2.len2()>0.000001&&hittmp1<t){
// 			t=hittmp1;
// 			fi=i;
// 		}
		
// 	}
// 	static double hittt[2];
// 	hittt[0]=fi;
// 	hittt[1]=t;
// 	return hittt[2];
// }

int hitidx(Ray r){
	double t=infinity;
	int fi=-1;
	for (int i = 0; i < scene_num; ++i){
		double hittmp1=scene[i]->hitnum(r);
		vec3 hittmp2=scene[i]->hitray(r);
		if(hittmp1<500000&&hittmp2.len2()>0.000001&&hittmp1<t){
			t=hittmp1;
			fi=i;
		}
		
	}
	return fi;
}
int hitt(Ray r){
	double t=infinity;
	int fi=-1;
	for (int i = 0; i < scene_num; ++i){
		double hittmp1=scene[i]->hitnum(r);
		vec3 hittmp2=scene[i]->hitray(r);
		if(hittmp1<500000&&hittmp2.len2()>0.000001&&hittmp1<t){
			t=hittmp1;
			fi=i;
		}
		
	}
	return t;
}

pair<int, double> hitrecord(Ray ray) {
	double t = INF;
	int id = -1;
	for (int i = 0; i < scene_num; ++i) {
		std::pair<double, vec3> tmp = scene[i]->hit(ray);
		if (tmp.first < INF / 2 && tmp.second.len2() > 1e-6 && tmp.first < t) {
			t = tmp.first;
			id = i;
		}
	}
	return std::make_pair(id, t);
}

Refl_t feaidx(Object* obj, Texture& texture, vec3 x){
	Refl_t feaidx;
	//feaidx=texture.getcol(x.z,x.x).first;
	feaidx=texture.getreflt(x.z,x.x);
	return feaidx;

}

vec3 feacol(Object* obj, Texture&texture, vec3 x){
	vec3 feacol;
	//feacol=texture.getcol(x.z,x.x).second;
	feacol=texture.getcolor(x.z,x.x);
	return feacol;
}





//std::vector<noderec> photontrac(Ray ray, int dep, int index, unsigned short* X, vec3 pref = vec3(1, 1, 1), double prob = 1.) {
vector<noderec> photontrac(Ray ray, int dep, int index, unsigned short* X, vec3 pref = vec3(1, 1, 1), double prob = 1.) {
// if index == -1 then the node is illegal
	vector<noderec> result;
	vector<noderec> tmp;

	if (pref.max() < 1e-6 || prob < 1e-6){
		return result;
	} 

	int into = 0;

	pair<int, double> hit_result = hitrecord(ray);
	int hit1=hitrecord(ray).first;
	double hit2=hitrecord(ray).second;
	//int hit1= int(hitidx(ray)[0]);
	//int hit1=hitidx(ray);
	//double hit2 = hitt(ray);

	//double hit2=hitrecord(ray).second;

	if (hit1 == -1){
		return result;
	}
	//Object* obj = scene[hit_result.first];

	Object* obj = scene[hit1];
	Texture& texture = obj->texture;
	//vec3 x = ray.get(hit_result.second);
	vec3 x = ray.get(hit2);

	// std::pair<Refl_t, vec3> feature = get_feature(obj, texture, x, X);
	// vec3 f = feature.second;

	//Refl_t fea1=get_feature(obj,texture,x).first;
	Refl_t fea1= feaidx(obj,texture,x);
	//vec3 fea2=get_feature(obj,texture,x).second;
	vec3 fea2=feacol(obj,texture,x);
	vec3 f=fea2;



	vec3 n = obj->norm(x);
	vec3 nl;

	//vec3 nl = n.dot(ray.d) < 0 ? into = 1, n : -n;

	if(n.dot(ray.d) < 0){
		 into = 1;
		 nl=n;
	}
	else{
		nl=-n;
	}


	double p = f.max();
	
	if (f.max() < 1e-6)
		return result;
	if (++dep > 5){
		if(randomm() < p) 
		{
			f /= p;
		}
		else 
		{
			return result;
		}
	}
		// if(randomm() < p) f /= p;
		// else return result;
	Ray reflray = Ray(x, ray.d.reflect(nl));
	// result.push_back(noderec(x, pref.com(f), nl, 1, index, prob));
	// return result;




	if (fea1 == DIFF ) { // vase: 0.8 prob
		result.push_back(noderec(x, pref.com(f), nl, 1, index, prob));
	}





	if (fea1 == SPEC) { // vase: 0.2 prob
		tmp = photontrac(reflray, dep, index, X, pref.com(f), prob);
		result.insert(result.end(), tmp.begin(), tmp.end());
	}





	if (fea1 == REFR) {

		double nt=1.5;
		double nc=1;

		double nnt; //= into ? nc/nt:nt/nc;

		if(into){
			nnt=nc/nt;
		}
		else{
			nnt=nt/nc;
		}


		//vec3 d = ray.d.refract(n, into ? 1 : texture.brdf, into ? texture.brdf : 1);
		vec3 d=ray.d.refractt(n,nnt);



		if (d.len2() < 1e-6) // Total internal reflection
			return photontrac(reflray, dep, index, X, pref.com(f), prob);



		double a = nt-nc;
		double b= nc-nt;

		double ddn=ray.d.dot(nl);

		double R0 = a * a / (b * b);
		double c0 = 1 - (into ? -ddn : d.dot(n));

			

		double Re = R0 + (1 - R0) * c0 * c0  * c0 * c0 * c0;
		double Tr = 1 - Re;
		double P = .25 + .5 * Re;
		double RP = Re / P;
		double TP = Tr / (1 - P);

		if (dep > 2)
			if (randomm() < P) {
				tmp = photontrac(reflray, dep, index, X, pref.com(f),  RP);
				result.insert(result.end(), tmp.begin(), tmp.end());
			}
			else {
				tmp = photontrac(Ray(x, d), dep, index, X, pref.com(f), TP);
				result.insert(result.end(), tmp.begin(), tmp.end());
			}
		else {
			tmp = photontrac(reflray, dep, index, X, pref.com(f),  Re);
			result.insert(result.end(), tmp.begin(), tmp.end());

			tmp = photontrac(Ray(x, d), dep, index, X, pref.com(f), Tr);
			result.insert(result.end(), tmp.begin(), tmp.end());
		}
	}
	return result;
}






//ray raytracing
void raytracing(Ray ray, int dep, vec3 col,unsigned short* X , vec2* c, Kdtree* tree, double prob = 1.0) {


	if (col.max() < 1e-6) 
	return;
	int into = 0;

	std::pair<int, double> hit_result = hitrecord(ray);

	int hit1=hitrecord(ray).first;
	int hit2 = hitrecord(ray).second;

	// int hit1=hitidx(ray);
	// double hit2 = hitt(ray);

	

	if (hit1 == -1)
		return;

	Object* obj = scene[hit1];

	Texture& texture = obj->texture;

	vec3 x = ray.get(hit2);

	//std::pair<Refl_t, vec3> feature = get_feature(obj, texture, x, X);

	//Refl_t fea1=get_feature(obj,texture,x).first;
	Refl_t fea1= feaidx(obj,texture,x);
	//vec3 fea2=get_feature(obj,texture,x).second;
	vec3 fea2=feacol(obj,texture,x);
	vec3 f=fea2;

	//vec3 f = f;
	vec3 n = obj->norm(x);
	vec3 nl;

	//vec3 nl = n.dot(ray.d) < 0 ? into = 1, n : -n;

	if(n.dot(ray.d) < 0){
		nl = n;
		into = 1;
	}
	else{
		nl = -n;
	}


	double p = f.max();


	if (f.max() < 1e-6) {
		tree->query(noderec(x, col, nl), c);
		return;
	}
	if (++dep > 5)
		if (randomm() < p) f /= p;
		else {
			tree->query(noderec(x, col, nl), c);
			return;
		}

	 //diffuse 	
	if (fea1 == DIFF) {

		tree->query(noderec(x, col, nl), c); // query col

		double r1 = 2 * PI * randomm();
		double r2 = randomm();
		double r2s = sqrt(r2);
		vec3 w = nl;

		vec3 u=((fabs(w.x) > .1 ? vec3(0, 1) : vec3(1)).cross(w)).norm();

		vec3 v = w.cross(u);
		vec3 d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
		return raytracing(Ray(x, d), dep, col.com(f), X, c, tree, prob);
	}
	else {
		Ray reflray = Ray(x, ray.d.reflect(nl));




		if (fea1 == SPEC) {

			double nt=1.5;
			double nc=1;

			double nnt; //= into ? nc/nt:nt/nc;

			if(into){
				nnt=nc/nt;
			}
			else{
				nnt=nt/nc;
			}

			// if(into){nnt=1;}else{nnt=nc/nt;}

			
			double tmp1 = into ? 1 : nt/nc;
			double tmp2 = into ? nt/nc : 1;

			// if (into){tmp1=1;}else{tmp1=nt/nc;}
			// if (into){tmp2=nt/nc;}else{tmp2=1;}

			vec3 d=ray.d.refractt(n,nnt);

			// vec3 d = ray.d.refract(n, into ? 1 : texture.brdf, into ? texture.brdf : 1);
			// vec3 d = ray.d.refract(n, tmp1, tmp2);
			// vec3 d =di*tmp1/tmp2-n*(norm().dot(n))

			// double nnt; //= into ? nc/nt:nt/nc;

			// if(into){
			// 	nnt=nc/nt;
			// }
			// else{
			// 	nnt=nt/nc;
			// }

			//if(into){nnt=1;}else{nnt=nc/nt;}



		

			if (d.len2() < 1e-6) // Total internal reflection
				return raytracing(reflray, dep, col.com(f), X, c, tree, prob);


		    double a = nt-nc;
			double b= nc-nt;

			double ddn=ray.d.dot(nl);

			double R0 = a * a / (b * b);
			double c0 = 1 - (into ? -ddn : d.dot(n));

			

			double Re = R0 + (1 - R0) * c0 * c0  * c0 * c0 * c0;
			double Tr = 1 - Re;
			double P = .25 + .5 * Re;
			double RP = Re / P;
			double TP = Tr / (1 - P);
			

			// return dep > 2 ? (randomm() < P ?   // Russian roulette
			// 	raytracing(reflray, dep, col.com(f), X, c, tree, prob * RP) : raytracing(Ray(x, d), dep, col.com(f), X, c, tree, prob * TP))
			//  : (raytracing(reflray, dep, col.com(f), X, c, tree, prob * Re),  raytracing(Ray(x, d), dep, col.com(f), X, c, tree, prob * Tr));

			 if(dep > 2)
			{
				if(randomm() < P)
				{
					return raytracing(reflray, dep, col.com(f),X, c, tree, prob * RP);
				}
				else
				{
					return raytracing(Ray(x, d), dep, col.com(f),X, c, tree, prob * TP);
				}
			}

			else
			{
				return (raytracing(reflray, dep, col.com(f),X, c, tree, prob * Re), raytracing(Ray(x, d), dep, col.com(f), X, c, tree, prob * Tr));
			}
		}
	}
}

#endif // __RENDER_H__
