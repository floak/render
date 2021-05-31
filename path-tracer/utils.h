#ifndef __UTILS_H__
#define __UTILS_H__

#include <omp.h>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <functional>
#include <random>

using namespace std;

const double pi = 3.14159265358979;
const double eps = 1e-6;
const double infinity = std::numeric_limits<double>::infinity();

inline double degrees_to_radians(double degrees) { return degrees * pi / 180;}

inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

static std::uniform_real_distribution<double> distribution;
static std::mt19937 generator;

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline double erand48() { return (double)rand() / (double)RAND_MAX; }

double gamma = 0.80;
double intensitymax = 255;

double* wavelengthtorgb(int Wavelength) {
    double factor;
    double Red, Green, Blue, Nt;
    //int Wavelength = atol(argv[1]);


    if ((Wavelength >= 380) && (Wavelength < 440)) {
        Red = -(Wavelength - 440) / (440 - 380);
        Green = 0.0;
        Blue = 1.0;
    }
    else if ((Wavelength >= 440) && (Wavelength < 490)) {
        Red = 0.0;
        Green = (Wavelength - 440) / (490 - 440);
        Blue = 1.0;
    }
    else if ((Wavelength >= 490) && (Wavelength < 510)) {
        Red = 0.0;
        Green = 1.0;
        Blue = -(Wavelength - 510) / (510 - 490);
    }
    else if ((Wavelength >= 510) && (Wavelength < 580)) {
        Red = (Wavelength - 510) / (580 - 510);
        Green = 1.0;
        Blue = 0.0;
    }
    else if ((Wavelength >= 580) && (Wavelength < 645)) {
        Red = 1.0;
        Green = -(Wavelength - 645) / (645 - 580);
        Blue = 0.0;
    }
    else if ((Wavelength >= 645) && (Wavelength < 781)) {
        Red = 1.0;
        Green = 0.0;
        Blue = 0.0;
    }
    else {
        Red = 0.0;
        Green = 0.0;
        Blue = 0.0;
    }

    if ((Wavelength >= 380) && (Wavelength < 420)) {
        factor = 0.3 + 0.7 * (Wavelength - 380) / (420 - 380);
    }
    else if ((Wavelength >= 420) && (Wavelength < 701)) {
        factor = 1.0;
    }
    else if ((Wavelength >= 701) && (Wavelength < 781)) {
        factor = 0.3 + 0.7 * (780 - Wavelength) / (780 - 700);
    }
    else {
        factor = 0.0;
    }

    static double rgb[4];
    rgb[0] = Red == 0.0 ? 0 : (int)floor(intensitymax * pow(Red * factor, gamma));
    rgb[1] = Green == 0.0 ? 0 : (int)floor(intensitymax * pow(Green * factor, gamma));
    rgb[2] = Blue == 0.0 ? 0 : (int)floor(intensitymax * pow(Blue * factor, gamma));
    rgb[3] = Nt = ((Wavelength - 380) / 1000) + 1.2;//折射率

    return rgb;
    //cout<< rgb<<

}


#endif
