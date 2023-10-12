#include "VPGen.hpp"
#include "Math.cpp"


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


API void setPrintInfo(void (*f)(int, long double)) {
    (*f)(10, 0.1);
}


API long double* generate(Domain domain, void (*f)(int, long double)) {
    srand(time(NULL));

    Assistant assistant;

    assistant.center = Point{
        domain.origin.x + domain.size.x / 2,
        domain.origin.y + domain.size.y / 2,
        domain.origin.z + domain.size.z / 2
    };

    std::cout <<
    "Origin: " <<
    domain.origin.x << ", " <<
    domain.origin.y << ", " <<
    domain.origin.z << ";" << std::endl <<
    "Size: " <<
    domain.size.x << ", " <<
    domain.size.y << ", " <<
    domain.size.z << ";" << std::endl <<
    "Indent: " <<
    domain.indent.x << ", " <<
    domain.indent.y << ", " <<
    domain.indent.z << ";" << std::endl <<
    "Periodicity: " <<
    domain.periodicity.x << ", " <<
    domain.periodicity.y << ", " <<
    domain.periodicity.z << ";" << std::endl <<
    "Radius: " <<
    domain.radius.min << " - " <<
    domain.radius.max << ";" << std::endl <<
    "Counter: " <<
    domain.counter.type << ", " <<
    domain.counter.number << ", " <<
    domain.counter.porosity << "%;" << std::endl <<
    "Minimum distance: " <<
    domain.minimum_distance << ";" << std::endl <<
    "Iterations: " <<
    domain.iterations << ";" << std::endl <<
    "Center: " <<
    assistant.center.x << ", " <<
    assistant.center.y << ", " <<
    assistant.center.z << ", " << std::endl;

    // check dimension
    if (domain.size.x == 0 && domain.size.y != 0 && domain.size.z != 0) {
        assistant.axises = Periodicity{false, true, true};
        assistant.volume = domain.size.y * domain.size.z;
        assistant.volume_full = domain.size.y * domain.size.z;
        assistant.dimension = 2;
    } else if (domain.size.x != 0 && domain.size.y == 0 && domain.size.z != 0) {
        assistant.axises = Periodicity{true, false, true};
        assistant.volume = domain.size.x * domain.size.z;
        assistant.volume_full = domain.size.x * domain.size.z;
        assistant.dimension = 2;
    } else if (domain.size.x != 0 && domain.size.y != 0 && domain.size.z == 0) {
        assistant.axises = Periodicity{true, true, false};
        assistant.volume = domain.size.x * domain.size.y;
        assistant.volume_full = domain.size.x * domain.size.y;
        assistant.dimension = 2;
    } else if (domain.size.x != 0 && domain.size.y != 0 && domain.size.z != 0) {
        assistant.axises = Periodicity{true, true, true};
        assistant.volume = domain.size.x * domain.size.y * domain.size.z;
        assistant.volume_full = domain.size.x * domain.size.y * domain.size.z;
        assistant.dimension = 3;
    } else {
        // raise error (wrong dimension)
    }

    std::list<Obstacle> obstacles = {};

    // start iterations
    for (int iteration = 0; iteration < domain.iterations; iteration++) {
        // create obstacle
        Obstacle obstacle = createObstacle(domain);
        // std::cout << "NEW OBSTACLE" << std::endl;

        // check intersection
        if (checkIntersection(domain, obstacles, obstacle)) continue;

        // check periodicity
        long double x_b = obstacle.center.x + 2 * std::copysignl(assistant.center.x - domain.origin.x, assistant.center.x - obstacle.center.x);
        long double y_b = obstacle.center.y + 2 * std::copysignl(assistant.center.y - domain.origin.y, assistant.center.y - obstacle.center.y);
        long double z_b = obstacle.center.z + 2 * std::copysignl(assistant.center.z - domain.origin.z, assistant.center.z - obstacle.center.z);

        bool x = assistant.axises.x && domain.periodicity.x;
        bool y = assistant.axises.y && domain.periodicity.y;
        bool z = assistant.axises.z && domain.periodicity.z;

        std::list<Obstacle> test_obstacles(obstacles);
        test_obstacles.insert(test_obstacles.end(), obstacle);

        long double created_obstacles_area = calculateDensity(assistant, domain, obstacle);

        if (x) {
            if (y) {
                if (z) {
                    Obstacle test_obstacle_xyz;
                    memcpy(&test_obstacle_xyz, &obstacle, sizeof(obstacle));
                    test_obstacle_xyz.center.x = x_b;
                    test_obstacle_xyz.center.y = y_b;
                    test_obstacle_xyz.center.z = z_b;
                    if (inside(domain, test_obstacle_xyz)) {
                        if (checkIntersection(domain, test_obstacles, test_obstacle_xyz)) continue;
                        test_obstacles.insert(test_obstacles.end(), test_obstacle_xyz);
                        created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_xyz);
                    }
                }
                Obstacle test_obstacle_xy;
                memcpy(&test_obstacle_xy, &obstacle, sizeof(obstacle));
                test_obstacle_xy.center.x = x_b;
                test_obstacle_xy.center.y = y_b;
                if (inside(domain, test_obstacle_xy)) {
                    if (checkIntersection(domain, test_obstacles, test_obstacle_xy)) continue;
                    test_obstacles.insert(test_obstacles.end(), test_obstacle_xy);
                    created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_xy);
                }
            }
            if (z) {
                Obstacle test_obstacle_xz;
                memcpy(&test_obstacle_xz, &obstacle, sizeof(obstacle));
                test_obstacle_xz.center.x = x_b;
                test_obstacle_xz.center.z = z_b;
                if (inside(domain, test_obstacle_xz)) {
                    if (checkIntersection(domain, test_obstacles, test_obstacle_xz)) continue;
                    test_obstacles.insert(test_obstacles.end(), test_obstacle_xz);
                    created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_xz);
                }
            }
            Obstacle test_obstacle_x;
            memcpy(&test_obstacle_x, &obstacle, sizeof(obstacle));
            test_obstacle_x.center.x = x_b;
            if (inside(domain, test_obstacle_x)) {
                if (checkIntersection(domain, test_obstacles, test_obstacle_x)) continue;
                test_obstacles.insert(test_obstacles.end(), test_obstacle_x);
                created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_x);
            }
        }
        if (y) {
            if (z) {
                Obstacle test_obstacle_yz;
                memcpy(&test_obstacle_yz, &obstacle, sizeof(obstacle));
                test_obstacle_yz.center.y = y_b;
                test_obstacle_yz.center.z = z_b;
                if (inside(domain, test_obstacle_yz)) {
                    if (checkIntersection(domain, test_obstacles, test_obstacle_yz)) continue;
                    test_obstacles.insert(test_obstacles.end(), test_obstacle_yz);
                    created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_yz);
                }
            }
            Obstacle test_obstacle_y;
            memcpy(&test_obstacle_y, &obstacle, sizeof(obstacle));
            test_obstacle_y.center.y = y_b;
            if (inside(domain, test_obstacle_y)) {
                if (checkIntersection(domain, test_obstacles, test_obstacle_y)) continue;
                test_obstacles.insert(test_obstacles.end(), test_obstacle_y);
                created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_y);
            }
        }
        if (z) {
            Obstacle test_obstacle_z;
            memcpy(&test_obstacle_z, &obstacle, sizeof(obstacle));
            test_obstacle_z.center.z = z_b;
            if (inside(domain, test_obstacle_z)) {
                if (checkIntersection(domain, test_obstacles, test_obstacle_z)) continue;
                test_obstacles.insert(test_obstacles.end(), test_obstacle_z);
                created_obstacles_area += calculateDensity(assistant, domain, test_obstacle_z);
            }
        }
        
        if (domain.counter.type == 0 && test_obstacles.size() > domain.counter.number) continue;
        obstacles = test_obstacles;
        assistant.volume -= created_obstacles_area;

        // std::cout << std::endl;
        assistant.porosity = assistant.volume / assistant.volume_full * 100;
        (*f)(obstacles.size(), assistant.porosity);

        // TODO check stop conditions
        if (domain.counter.type == 0) {
            // by number
            if (obstacles.size() == domain.counter.number) {
                break;
            }
        } else if (domain.counter.type == 1) {
            // by porosity
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
                // raise error
            }
        }
    }

    // done
    OBSTACLES = obstacles;

    long double* res = new long double[2];
    res[0] = (long double) OBSTACLES.size();
    res[1] = assistant.porosity;

    return res;
}


API Obstacle* getObstacles() {
    Obstacle* obstacles = new Obstacle[OBSTACLES.size()];
    int i = 0;
    for ( Obstacle obstacle : OBSTACLES ) {
        obstacles[i] = obstacle;
        i++;
    }
    return obstacles;
}
