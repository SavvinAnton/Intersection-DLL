#include "Maths.cpp"
#include "VPGen.h"


std::list < Obstacle > OBSTACLES = {};


API void clear() {
	OBSTACLES.clear();
}


API int length() {
	return OBSTACLES.size();
}


API Obstacle newObstacle( Rect domain, Radius radius ) {
	Obstacle obstacle;
	obstacle.point.x = fRand(domain.origin.x, domain.origin.x + domain.size.x);
	obstacle.point.y = fRand(domain.origin.y, domain.origin.y + domain.size.y);
	obstacle.point.z = fRand(domain.origin.z, domain.origin.z + domain.size.z);
	obstacle.r = fRand(radius.min, radius.max);
	return obstacle;
}


API Obstacle *getObstacles() {
	int size = OBSTACLES.size();
	Obstacle *container = new Obstacle[size];
	int index = 0;
	for ( Obstacle obstacle : OBSTACLES ) {
		container[index] = obstacle;
		index++;
	}
	return container;
}


API bool intersection( long double x_new, long double y_new, long double z_new, long double r_new, long double min_dist ) {
	for ( Obstacle obstacle : OBSTACLES ) {
		if ( distance( obstacle.point.x, obstacle.point.y, obstacle.point.z, x_new, y_new, z_new ) <= ( obstacle.r + r_new + min_dist ) ) {
			return true;
		}
	}
	return false;
}


API bool addObstacle( Obstacle new_obstacle, long double min_dist ) {
	if ( !intersection( new_obstacle.point.x, new_obstacle.point.y, new_obstacle.point.z, new_obstacle.r, min_dist ) ) {
		OBSTACLES.push_back(new_obstacle);
		return true;
	}
	return false;
}


API void justAddObstacle( Obstacle new_obstacle ) {
	OBSTACLES.push_back(new_obstacle);
}


API bool tryObstacle( Obstacle new_obstacle, long double min_dist ) {
	return intersection( new_obstacle.point.x, new_obstacle.point.y, new_obstacle.point.z, new_obstacle.r, min_dist );
}


API Obstacle mirror( Rect domain, Obstacle obstacle, char axis ) {
	Obstacle new_obstacle = { obstacle.point.x, obstacle.point.y, obstacle.point.z };
	*new_obstacle.point(axis) = 2 * (*domain.origin(axis)) + *domain.size(axis) - *obstacle.point(axis);
	return new_obstacle;
}


API void generate( Rect domain, Count count, Radius radius, Period period, Indent indent, long double min_dist, unsigned int max_iterations ) {
	domain.origin.x += indent.x;
	domain.origin.y += indent.y;
	domain.origin.z += indent.z;

	Point domain_center = {domain.origin.x + domain.size.x / 2, domain.origin.y + domain.size. y/ 2, domain.origin.z + domain.size.z / 2};

	for ( int iteration = 1; iteration < max_iterations + 1; iteration++ ) {
		Obstacle new_obstacle = newObstacle(domain, radius);
		Obstacle x_obstacle = mirror(domain, new_obstacle, 'x');
		Obstacle y_obstacle = mirror(domain, new_obstacle, 'y');
		Obstacle z_obstacle = mirror(domain, new_obstacle, 'z');
		CheckPeriod check_period = { 
			domain.size.x == 0 || !period.x || tryObstacle( x_obstacle, min_dist ),
			domain.size.y == 0 || !period.y || tryObstacle( y_obstacle, min_dist ),
			domain.size.z == 0 || !period.z || tryObstacle( z_obstacle, min_dist )
		};
		if ( check_period.x && check_period.y && check_period.z ) {
			justAddObstacle(new_obstacle);
			if ( period.x ) { justAddObstacle(x_obstacle); }
			if ( period.y ) { justAddObstacle(y_obstacle); }
			if ( period.z ) { justAddObstacle(z_obstacle); }
		}
	}
}
