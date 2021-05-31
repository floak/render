#ifndef __UTILS_H__
#define __UTILS_H__

#include <bits/stdc++.h>
#include <omp.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION
using namespace std;


const double PI = 3.14159265358979;

const double INF = 1 << 20;
const double infinity = numeric_limits<double>::infinity();


enum Refl_t { DIFF, SPEC, REFR };

int gamma_trans(double x) {
   
    double tmp;
    if(x<0){
       tmp=0;
    }
    else {
        if(x>1){
            tmp=1;
        }
        else{tmp=x;}
    }
    return int(255*pow(tmp,1/2.2)+0.5);
}

double sqr(double x) {
    return x * x;
    }

#endif // __UTILS_H__
