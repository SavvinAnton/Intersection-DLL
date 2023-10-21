#include <include/VPGen.h>
#include <include/VPMath.h>


std::list<Obstacle> OBSTACLES;


inline long double frand(long double min, long double max) {
    return min + static_cast<long double>(rand()) / ( static_cast<long double>(RAND_MAX/(max - min)));
}


inline Obstacle createObstacle(Domain domain) {
    return Obstacle{
        Point{
            domain.origin.x + domain.indent.x + frand(0, domain.size.x),
            domain.origin.y + domain.indent.y + frand(0, domain.size.y),
            domain.origin.z + domain.indent.z + frand(0, domain.size.z)
        },
        frand(domain.radius.min, domain.radius.max)
    };
}


inline long double distance(Obstacle obstacle1, Obstacle obstacle2) {
    return std::sqrt(
        std::pow(obstacle1.center.x - obstacle2.center.x, 2) +
        std::pow(obstacle1.center.y - obstacle2.center.y, 2) +
        std::pow(obstacle1.center.z - obstacle2.center.z, 2)
    );
}


inline bool checkIntersection(Domain domain, std::list<Obstacle> obstacles, Obstacle obstacle1) {
    for ( Obstacle obstacle2 : obstacles ) {
        if (distance(obstacle1, obstacle2) < (obstacle1.radius + obstacle2.radius + domain.minimum_distance)) {
            return true;
        }
    }
    return false;
}


inline bool inside(Domain domain, Obstacle obstacle) {
    if (domain.origin.x + domain.indent.x > obstacle.center.x + obstacle.radius ||
        domain.origin.y + domain.indent.y > obstacle.center.y + obstacle.radius ||
        domain.origin.z + domain.indent.z > obstacle.center.z + obstacle.radius ||
        domain.origin.x + domain.size.x + domain.indent.x < obstacle.center.x - obstacle.radius ||
        domain.origin.y + domain.size.y + domain.indent.y < obstacle.center.y - obstacle.radius ||
        domain.origin.z + domain.size.z + domain.indent.z < obstacle.center.z - obstacle.radius) {
            return false;
        }
    return true;
}


API_def long double* generate(Domain domain, void (*f)(int, long double)) {
    srand(time(NULL));

    Assistant assistant;

    assistant.center = Point{
        domain.origin.x + domain.size.x / 2,
        domain.origin.y + domain.size.y / 2,
        domain.origin.z + domain.size.z / 2
    };

    if (domain.size.x != 0.0 && domain.size.y != 0.0 && domain.size.z == 0.0) {
        assistant.density_area = domain.size.x * domain.size.y;
        assistant.dimension = 2;
    } else if (domain.size.x != 0.0 && domain.size.y != 0.0 && domain.size.z != 0.0) {
        assistant.density_area = domain.size.x * domain.size.y * domain.size.z;
        assistant.dimension = 3;
    } else {
        throw std::invalid_argument("There is only (xy) or (xyz) supported!");
    }

    std::list<Obstacle> obstacles = {};
    assistant.density_obstacles = 0.0;

    for (int iteration = 0; iteration < domain.iterations; iteration++) {
        Obstacle obstacle = createObstacle(domain);

        if (checkIntersection(domain, obstacles, obstacle)) continue;

        long double x_b = obstacle.center.x + 2 * std::copysignl(assistant.center.x - domain.origin.x, assistant.center.x - obstacle.center.x);
        long double y_b = obstacle.center.y + 2 * std::copysignl(assistant.center.y - domain.origin.y, assistant.center.y - obstacle.center.y);
        long double z_b = obstacle.center.z + 2 * std::copysignl(assistant.center.z - domain.origin.z, assistant.center.z - obstacle.center.z);

        std::list<Obstacle> test_obstacles(obstacles);
        test_obstacles.insert(test_obstacles.end(), obstacle);

        long double created_obstacles_area = calculateDensity(assistant, domain, obstacle);

        if (domain.periodicity.x) {
            if (domain.periodicity.y) {
                if (domain.periodicity.z) {
                    Obstacle test_obstacle_xyz = obstacle;
                    test_obstacle_xyz.center.x = x_b;
                    test_obstacle_xyz.center.y = y_b;
                    test_obstacle_xyz.center.z = z_b;
                    if (inside(domain, test_obstacle_xyz)) {
                        if (checkIntersection(domain, test_obstacles, test_obstacle_xyz)) continue;
                        test_obstacles.insert(test_obstacles.end(), test_obstacle_xyz);
                        created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_xyz);
                    }
                }
                Obstacle test_obstacle_xy = obstacle;
                test_obstacle_xy.center.x = x_b;
                test_obstacle_xy.center.y = y_b;
                if (inside(domain, test_obstacle_xy)) {
                    if (checkIntersection(domain, test_obstacles, test_obstacle_xy)) continue;
                    test_obstacles.insert(test_obstacles.end(), test_obstacle_xy);
                    created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_xy);
                }
            }
            if (domain.periodicity.z) {
                Obstacle test_obstacle_xz = obstacle;
                test_obstacle_xz.center.x = x_b;
                test_obstacle_xz.center.z = z_b;
                if (inside(domain, test_obstacle_xz)) {
                    if (checkIntersection(domain, test_obstacles, test_obstacle_xz)) continue;
                    test_obstacles.insert(test_obstacles.end(), test_obstacle_xz);
                    created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_xz);
                }
            }
            Obstacle test_obstacle_x = obstacle;
            test_obstacle_x.center.x = x_b;
            if (inside(domain, test_obstacle_x)) {
                if (checkIntersection(domain, test_obstacles, test_obstacle_x)) continue;
                test_obstacles.insert(test_obstacles.end(), test_obstacle_x);
                created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_x);
            }
        }
        if (domain.periodicity.y) {
            if (domain.periodicity.z) {
                Obstacle test_obstacle_yz = obstacle;
                test_obstacle_yz.center.y = y_b;
                test_obstacle_yz.center.z = z_b;
                if (inside(domain, test_obstacle_yz)) {
                    if (checkIntersection(domain, test_obstacles, test_obstacle_yz)) continue;
                    test_obstacles.insert(test_obstacles.end(), test_obstacle_yz);
                    created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_yz);
                }
            }
            Obstacle test_obstacle_y = obstacle;
            test_obstacle_y.center.y = y_b;
            if (inside(domain, test_obstacle_y)) {
                if (checkIntersection(domain, test_obstacles, test_obstacle_y)) continue;
                test_obstacles.insert(test_obstacles.end(), test_obstacle_y);
                created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_y);
            }
        }
        if (domain.periodicity.z) {
            Obstacle test_obstacle_z = obstacle;
            test_obstacle_z.center.z = z_b;
            if (inside(domain, test_obstacle_z)) {
                if (checkIntersection(domain, test_obstacles, test_obstacle_z)) continue;
                test_obstacles.insert(test_obstacles.end(), test_obstacle_z);
                created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_z);
            }
        }
        
        if (domain.counter.type == 0 && test_obstacles.size() > domain.counter.number) continue;
        iteration = 0;
        obstacles = test_obstacles;
        assistant.density_obstacles += created_obstacles_area;

        assistant.porosity = (assistant.density_area - assistant.density_obstacles) / assistant.density_area * 100;
        (*f)(obstacles.size(), assistant.porosity);

        if (domain.counter.type == 0) {
            if (obstacles.size() == domain.counter.number) {
                break;
            }
        } else if (domain.counter.type == 1) {
            if (domain.counter.porosity > assistant.porosity) {
                break;
            }
        } else {
            if (domain.counter.number > 0) {
                domain.counter.type = 0;
                std::cout << "domain.counter.type is not defined\nswitching to \"by number\"" << std::endl;
            } else if (domain.counter.porosity > 0 && domain.counter.porosity < 100) {
                domain.counter.type = 1;
                std::cout << "domain.counter.type is not defined\nswitching to \"by porosity\"" << std::endl;
            } else {
                throw std::invalid_argument("ERROR: domain.counter.type is wrong!");
            }
        }
    }

    OBSTACLES = obstacles;

    long double* res = new long double[2];
    res[0] = (long double) OBSTACLES.size();
    res[1] = assistant.porosity;

    return res;
}


API_def Obstacle* getObstacles() {
    Obstacle* obstacles = new Obstacle[OBSTACLES.size()];
    int i = 0;
    for (Obstacle obstacle : OBSTACLES) {
        obstacles[i] = obstacle;
        i++;
    }
    return obstacles;
}
