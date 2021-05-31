#include "render.hpp"
using namespace std;


int main(int argc, char* argv[])
{
	int w = atoi(argv[1]), h = atoi(argv[2]), iter = atoi(argv[4]);
	double rad = atof(argv[6]), alpha = atof(argv[7]), samp = atof(argv[5]) * w * h;

	


	//Ray cam(vec3(427,1000,447),vec3(-1,-2,-1.5).norm());

	Ray cam(vec3(150, 28, 260), vec3(-0.45, 0.001, -1).norm()); //pos,dir
	//int 16 = omp_get_num_procs(); 

	//for (int i=scene_num;i>0; i--){scene_num=i;}
	scene_num-=1;

	vec3 cx = vec3(w * .33 / h);
	vec3 cy=(cx & vec3(cam.d.x, 0, cam.d.z)).norm() * .33;

	vec2 **c = new vec2*[16];

	for (int i = 0; i < 16; ++i)
		c[i] = new vec2[h * w];

	vec2* final = new vec2[h * w];
	vec2* now = new vec2[h * w];

	cx *= 1.05;
	double aperture = .0;

	
	// for (int _ = 1; _ <= iter; fprintf(stderr, "\niter %d done!\n", _), ++_) {

	// buidouble kdtree
	std::vector<noderec> ball[16];
	Kdtree tree;
	for (int _ = 1; _ <= iter; fprintf(stderr, "\riter %d done!\n", _), ++_) {
		if (_ < 3 || _ % 1 == 0) {
			if (_ > 1) {
				samp /= sqrt(alpha);
				rad *= alpha;
			}
			#pragma omp parallel for num_threads(16) schedule(dynamic, 1)
			for (int y = 0; y < h; ++y) {
				int num = omp_get_thread_num();
				fprintf(stderr, "\rbuid kdtree %5.2f%% ... ", 100. * y / h);
				for (int x = 0; x < w; ++x)
				for (int sy = 0; sy < 2; ++sy)
				for (int sx = 0; sx < 2; ++sx) {
					unsigned short X[3] = {y + sy, y * x * time(0) + sx, y * x * y + time(0) + sy * 2 + sx};
					double r1 = 2 * erand48(X), dx = r1 < 1 ? sqrt(r1): 2-sqrt(2-r1);
					double r2 = 2 * erand48(X), dy = r2 < 1 ? sqrt(r2): 2-sqrt(2-r2);
					vec3 d = cx * ((dx / 2 + x + sx) / w - .5) + cy * ((dy / 2 + y + sy) / h - .5) + cam.d;
					vec3 pp = cam.o + d * 150, loc = cam.o + (vec3(erand48(X) * 1.05, erand48(X)) - .5) * 2 * aperture;
					vector<noderec> tmp = photontrac(Ray(pp, (pp - loc).norm()), 0, y * w + x, X);


					for (int i = 0; i < tmp.size(); ++i)
						if (tmp[i].index >= 0) {
							tmp[i].r = rad;
							ball[num].push_back(tmp[i]);
						}
				}
			}
			std::vector<noderec> totball;
			fprintf(stderr, "\rbuid tree ... ");
			for(int i = 0; i < 16; ++i) {
				totball.insert(totball.end(), ball[i].begin(), ball[i].end());
				// printf("%d: %d\n", i, ball[i].size());
			}
			tree.init(totball);
			fprintf(stderr, "done.\n");
		}

		fprintf(stderr, "rad = %f samp = %.0f\n", rad, samp);
		int per = samp / 16 + 1;
		#pragma omp parallel for num_threads(16) schedule(dynamic, 1)
		for (int t = 0; t < 16; ++t) {
			unsigned short X[3] = {t, t * t, (t & (t * t)) + _ + time(0)};
			int num = omp_get_thread_num();
			// for (int i = 0; i < totball.size(); ++i)
			// 	tree.query(totball[i], c[num]);
			// for (double y = 0; y <= 81; y += .5)
			// 	for (double x = 1; x < 200; x += .5)
			// 		for (double z = 0; z < 150; z += .5)
			// 		tree.query(noderec(vec3(x, y, z), vec3(1, 1, 1), vec3(0,0,0)), c[num]);
			for (int __ = 0; __ < per; __++) {
				if (num == 0 && __ % 1000 == 0)
					fprintf(stderr, "\rsppm tracing %5.2f%%", 100. * __ / per);
				// gen random light
				double rc = erand48(X) * 18;
				double tht = erand48(X) * 2 * PI;
				vec3 o(50 + rc * cos(tht), 81.6 - 1e-6, 81.6 + rc * sin(tht));
				double r1 = 2 * PI * erand48(X);
				double r2 = erand48(X);
				double r2s = sqrt(r2);
				vec3 w = vec3(0, -1, 0);
				vec3 u=(vec3(1).cross(w)).norm();
				vec3 v = w.cross(u);
				vec3 d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
				Ray light = Ray(o, d);
				vec3 col = vec3(1, 1, 1) + .4;
				tree.query(noderec(o, col, d), c[num]);
				raytracing(light, 0, col,X, c[num], &tree);
			}
		}
		//gather result
		memset(now, 0, sizeof now);
		for (int i = 0; i < 16; memset(c[i], 0, sizeof c[i]), ++i)
			for (int j = h * w - 1; j >= 0; --j)
				now[j] += c[i][j];
	
		for (int i = h * w - 1; i >= 0; --i)
			final[i] += now[i] / now[i].n;// * alpha;
		if (_ == 1 || _ % 1 == 0) {
			char sout[100];
			sprintf(sout, "%s%03d.ppm", argv[3], _);
			FILE* f = fopen(sout, "w");
			fprintf(f, "P6\n%d %d\n%d\n", w, h, 255);
			for (int y = h - 1; y >= 0; --y)
				for (int x = w - 1; x >= 0; --x) {
					fprintf(f, "%c%c%c", gamma_trans(final[y*w+x].get().x), gamma_trans(final[y*w+x].get().y), gamma_trans(final[y*w+x].get().z));
				}
			fclose(f);
		}
	}
	return !puts("");
}


