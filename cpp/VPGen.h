#pragma once

#include <iostream>
#include <cmath>
#include <list>
#include <time.h>

#define API
#define API_def extern "C" API

API_def struct Point {
	long double x, y, z;
	long double *operator()( char axis ) {
        if ( axis != 'x' && axis != 'y' && axis != 'z' ) {
            throw std::invalid_argument("Received invalid argument to Point()");
        }
		if ( axis == 'x' ) return &x;
		if ( axis == 'y' ) return &y;
		if ( axis == 'z' ) return &z;
	}
};

API_def struct Obstacle {
	Point point;
	long double r;
};

API_def struct Rect {
	Point origin, size;
};

API_def struct Radius {
	long double min, max;
};

API_def struct Period {
	bool x, y, z;
};

API_def struct CheckPeriod {
	bool x, y, z, xy, xz, yz, xyz;
};

API_def struct Count {
	short type;
	unsigned int obstacles;
	long double porosity;
};

API_def struct Indent {
	long double x, y, z;
};

API_def void clear();

API_def Obstacle *getObstacles();

API_def int length();

API_def void generate( Rect domain, Count count, Radius radius, Period period, Indent indent, long double min_dist, unsigned int max_iterations );
