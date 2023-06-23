#include <cmath>
#include <time.h>


long double distance( long double x1, long double y1, long double z1, long double x2, long double y2, long double z2 ) {
	return ( long double ) sqrt( ( x2 - x1 ) * ( x2 - x1 ) + ( y2 - y1 ) * ( y2 - y1 ) + ( z2 - z1 ) * ( z2 - z1 ) );
}

void updateSrand() {
	srand( time( NULL ) );
}

long double fRand( long double min, long double max ) {
	return min + ( ( long double ) rand() / RAND_MAX ) * (max - min);
}
