#pragma once

#ifdef __linux__
    #define decl
#elif _WIN32
    #define decl __declspec(dllexport)
#else
    #define decl
#endif

#define API decl
#define API_def extern "C" API
#define _USE_MATH_DEFINES
#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include <cstring>
#include <time.h>


struct Point {
    long double x, y, z;
};


struct Periodicity {
    bool x, y, z;
};


struct Radius {
    long double min, max;
};


struct Counter {
    short type;
    unsigned int number;
    long double porosity;
};


struct Domain {
    int dimension;
    Point origin;
    Point size;
    Point indent;
    Periodicity periodicity;
    Radius radius;
    Counter counter;
    long double minimum_distance;
    unsigned int iterations;
    bool exact_count;
    short order;
};


struct Obstacle {
    Point center;
    long double radius;
};


struct Assistant {
    Point center;
    Point min;
    Point max;
    long double porosity, density_obstacles, density_area;
    int dimension;
};


API_def long double* generate(Domain domain, void (*f)(int, long double));
API_def Obstacle* getObstacles();
