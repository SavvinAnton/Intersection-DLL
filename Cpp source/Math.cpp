#include "VPGen.hpp"


inline long double areaIntegral(Obstacle obstacle, long double x) {
    long double arcsin, eps = 1e-12, value = (x - obstacle.center.x) / obstacle.radius, square_root = std::pow(obstacle.radius, 2) - std::pow(x, 2) + 2 * obstacle.center.x * x - std::pow(obstacle.center.x, 2);
    if (1 - eps / 2 < value && 1 + eps / 2 > value) arcsin = M_PI / 2;
    else if (-1 - eps / 2 < value && -1 + eps / 2 > value) arcsin = - M_PI / 2;
    else arcsin = asin(value);
    if (-eps / 2 < square_root && square_root < eps / 2) square_root = 0.0;
    return std::pow(obstacle.radius, 2) * arcsin + (x - obstacle.center.x) * std::sqrt(square_root);
}


inline long double change(long double x, long double r) {
    return r * std::cos(std::asin(x / r));
}


inline long double getArea(Assistant assistant, Domain domain, Obstacle obstacle) {
    long double area = 0.0;
    if (obstacle.radius == 0.0) return area;
    if ((obstacle.center.x - obstacle.radius) > domain.indent.x + domain.origin.x && (obstacle.center.y - obstacle.radius) > domain.indent.y + domain.origin.y && (obstacle.center.x + obstacle.radius) < (domain.indent.x + domain.origin.x + domain.size.x) && (obstacle.center.y + obstacle.radius) < (domain.indent.y + domain.origin.y + domain.size.y)) area = M_PI * std::pow(obstacle.radius, 2);
    else {
        long double left_bound, right_bound;
        bool touches_bottom, touches_top, touches_left, touches_right;

        if (touches_left = (domain.indent.x + domain.origin.x) > (obstacle.center.x - obstacle.radius)) left_bound = domain.indent.x + domain.origin.x;
        else left_bound = obstacle.center.x - obstacle.radius;

        if (touches_right = (domain.indent.x + domain.origin.x + domain.size.x) < (obstacle.center.x + obstacle.radius)) right_bound = domain.indent.x + domain.origin.x + domain.size.x;
        else right_bound = obstacle.center.x + obstacle.radius;

        touches_bottom = (obstacle.center.y + obstacle.radius) >= (domain.indent.y + domain.origin.y) && (domain.indent.y + domain.origin.y) >= (obstacle.center.y - obstacle.radius);
        touches_top = (obstacle.center.y + obstacle.radius) >= (domain.indent.y + domain.origin.y + domain.size.y) && (domain.indent.y + domain.origin.y + domain.size.y) >= (obstacle.center.y - obstacle.radius);
        
        if (!touches_bottom && !touches_top) area = 2 * (areaIntegral(obstacle, right_bound) - areaIntegral(obstacle, left_bound));
        else if (touches_bottom && !touches_top) {
            long double left, right, square_root, h = domain.indent.y + obstacle.center.y - domain.origin.y;
            square_root = sqrt(std::pow(obstacle.radius, 2) - std::pow(h, 2));
            left = obstacle.center.x - square_root;
            right = obstacle.center.x + square_root;
            area = areaIntegral(obstacle, right_bound) - areaIntegral(obstacle, left_bound) + areaIntegral(obstacle, left) - areaIntegral(obstacle, left_bound) + areaIntegral(obstacle, right_bound) - areaIntegral(obstacle, right) + square_root * 2 * h;
        
        } else if (!touches_bottom && touches_top) {
            long double left, right, square_root, h = domain.indent.y + domain.origin.y + domain.size.y - obstacle.center.y;
            square_root = sqrt(std::pow(obstacle.radius, 2) - std::pow(h, 2));
            left = obstacle.center.x - square_root;
            right = obstacle.center.x + square_root;
            area = areaIntegral(obstacle, right_bound) - areaIntegral(obstacle, left_bound) + areaIntegral(obstacle, left) - areaIntegral(obstacle, left_bound) + areaIntegral(obstacle, right_bound) - areaIntegral(obstacle, right) + square_root * 2 * h;
        }
    }
    return area;
}


inline long double getVolume(Assistant assistant, Domain domain, Obstacle obstacle) {
    long double volume = 0.0;
    if ((obstacle.center.x - obstacle.radius) > (domain.indent.x + domain.origin.x) && (obstacle.center.y - obstacle.radius) > (domain.indent.y + domain.origin.y) && (obstacle.center.z - obstacle.radius) > (domain.indent.z + domain.origin.z) && (obstacle.center.x + obstacle.radius) < (domain.indent.x + domain.origin.x + domain.size.x) && (obstacle.center.y + obstacle.radius) < (domain.indent.y + domain.origin.y + domain.size.y) && (obstacle.center.z + obstacle.radius) < (domain.indent.z + domain.origin.z + domain.size.z)) volume = 4.0 / 3.0 * M_PI * std::pow(obstacle.radius, 3);
    else {
        long double back_radius = 0.0, front_radius = 0.0;
        bool touches_back, touches_front;

        if (touches_back = (obstacle.center.z - obstacle.radius) < (domain.origin.z + domain.indent.z)) back_radius = change(obstacle.center.z - domain.origin.z - domain.indent.z, obstacle.radius);
        if (touches_front = (obstacle.center.z + obstacle.radius) > (domain.origin.z + domain.indent.z + domain.size.z)) front_radius = change(domain.origin.z + domain.indent.z + domain.size.z - obstacle.center.z, obstacle.radius);

        int parts = 10;
        if (touches_back && touches_front) {
            Obstacle obstacle_back = obstacle, obstacle_front = obstacle;
            obstacle_back.radius = back_radius;
            obstacle_front.radius = front_radius;
            long double dr_back = (obstacle.radius - back_radius) / parts, dr_front = (obstacle.radius - front_radius) / parts, sum_back = getArea(assistant, domain, obstacle) / 2 + getArea(assistant, domain, obstacle_back) / 2, sum_front = getArea(assistant, domain, obstacle) / 2 + getArea(assistant, domain, obstacle_front) / 2;
            for (long double r_back = back_radius + dr_back, r_front = front_radius + dr_front; r_back < back_radius && r_front < front_radius; r_back += dr_back, r_front += dr_front) {
                obstacle_back.radius = r_back;
                obstacle_front.radius = r_front;
                sum_back += getArea(assistant, domain, obstacle_back);
                sum_front += getArea(assistant, domain, obstacle_front);
            }
            volume += sum_back * dr_back + sum_front * dr_front;
        } else if (touches_back) {
            Obstacle obstacle_back = obstacle;
            long double dr_back = (obstacle.radius - back_radius) / parts, sum_back = getArea(assistant, domain, obstacle) / 2 + getArea(assistant, domain, obstacle_back) / 2;
            for (long double r_back = back_radius + dr_back; r_back < back_radius; r_back += dr_back) {
                obstacle_back.radius = r_back;
                sum_back += getArea(assistant, domain, obstacle_back);
            }
            volume += sum_back * dr_back + 2 / 3 * M_PI * std::pow(obstacle.radius, 3);
        } else if (touches_front) {
            Obstacle obstacle_front = obstacle;
            long double dr_front = (obstacle.radius - front_radius) / parts, sum_front = getArea(assistant, domain, obstacle) / 2 + getArea(assistant, domain, obstacle_front) / 2;
            for (long double r_front = front_radius + dr_front; r_front < front_radius; r_front += dr_front) {
                obstacle_front.radius = r_front;
                sum_front += getArea(assistant, domain, obstacle_front);
            }
            volume += sum_front * dr_front + 2 / 3 * M_PI * std::pow(obstacle.radius, 3);
        }
    }
    return volume;
}


inline long double calculateDensity(Assistant assistant, Domain domain, Obstacle obstalce) {
    if (assistant.dimension == 2) {
        return getArea(assistant, domain, obstalce);
    } else if (assistant.dimension == 3) {
        return getVolume(assistant, domain, obstalce);
    } else {
        throw std::invalid_argument("ERROR: assistant.dimension is wrong!");
    }
}
