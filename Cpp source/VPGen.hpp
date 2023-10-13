#pragma once

#define API
#define API_def extern "C" API

#include <iostream>
#include <list>
#include <cmath>
#include <cstring>

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
};


struct Obstacle {
    Point center;
    long double radius;
};


struct Assistant {
    Point center;
    Point min;
    Point max;
    long double porosity, volume, volume_full;
    int dimension;
};


API_def long double* generate(Domain domain, void (*f)(int, long double));
API_def Obstacle* getObstacles();
API_def void setPrintInfo(void (*f)(int, long double));
