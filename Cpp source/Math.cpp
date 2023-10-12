#pragma once

#define _USE_MATH_DEFINES
#include "VPGen.hpp"


inline long double areaIntegral(Obstacle obstacle, long double x) {
    long double eps = 1e-7, value = (x - obstacle.center.x) / obstacle.radius, arcsin;
    if (1 - eps / 2 < value && 1 + eps / 2 > value) {
        arcsin = M_PI / 2;
    } else if (-1 - eps < value && -1 + eps > value) {
        arcsin = - M_PI / 2;
    } else {
        arcsin = asin(value);
    }
    long double square_root = std::pow(
                    obstacle.radius, 2
                ) - std::pow(
                    x, 2
                ) + 2 * obstacle.center.x * x - std::pow(
                    obstacle.center.x, 2
                );
    if (-eps / 2 < square_root && square_root < eps / 2) {
        square_root = 0;
    }
    return (
        std::pow(
            obstacle.radius, 2
        ) * arcsin + (
            x - obstacle.center.x
        ) * (
            std::sqrt(square_root)
        )
    );
}


inline long double volumeIntegral(Domain domain, Obstacle obstacle, long double x) {
    return 0.0;
}


inline long double getArea(Assistant assistant, Domain domain, Obstacle obstacle) {
    long double area = 0.0;
    if ((obstacle.center.x - obstacle.radius) > domain.indent.x + domain.origin.x &&
        (obstacle.center.y - obstacle.radius) > domain.indent.y + domain.origin.y &&
        (obstacle.center.x + obstacle.radius) < (domain.indent.x + domain.origin.x + domain.size.x) &&
        (obstacle.center.y + obstacle.radius) < (domain.indent.y + domain.origin.y + domain.size.y)) {
            area = M_PI * std::pow(obstacle.radius, 2);
            // std::cout << "FULL" << std::endl;
    } else {
        long double left_bound, right_bound;
        bool touches_bottom, touches_top, touches_left, touches_right;
        if ((domain.indent.x + domain.origin.x) < (obstacle.center.x - obstacle.radius)) {
            touches_left = false;
            left_bound = obstacle.center.x - obstacle.radius;
        } else {
            touches_left = true;
            left_bound = domain.indent.x + domain.origin.x;
        }
        if ((domain.indent.x + domain.origin.x + domain.size.x) > (obstacle.center.x + obstacle.radius)) {
            touches_right = false;
            right_bound = obstacle.center.x + obstacle.radius;
        } else {
            touches_right = true;
            right_bound = domain.indent.x + domain.origin.x + domain.size.x;
        }
        touches_bottom = (obstacle.center.y + obstacle.radius) >= (domain.indent.y + domain.origin.y) && (domain.indent.y + domain.origin.y) >= (obstacle.center.y - obstacle.radius);
        touches_top = (obstacle.center.y + obstacle.radius) >= (domain.indent.y + domain.origin.y + domain.size.y) && (domain.indent.y + domain.origin.y + domain.size.y) >= (obstacle.center.y - obstacle.radius);
        // std::cout << touches_left << touches_top << touches_right << touches_bottom << std::endl;
        if (!touches_bottom && !touches_top) {
            area = 2 * (areaIntegral(obstacle, right_bound) - areaIntegral(obstacle, left_bound));
        } else if (touches_bottom && !touches_top) {
            long double left, right, square_root, h = domain.indent.y + obstacle.center.y - domain.origin.y;
            square_root = sqrt(std::pow(obstacle.radius, 2) - std::pow(h, 2));
            left = obstacle.center.x - square_root;
            right = obstacle.center.x + square_root;
            area = areaIntegral(obstacle, right_bound) - areaIntegral(obstacle, left_bound);
            area += areaIntegral(obstacle, left) - areaIntegral(obstacle, left_bound);
            area += areaIntegral(obstacle, right_bound) - areaIntegral(obstacle, right);
            area += square_root * 2 * h;
        } else if (!touches_bottom && touches_top) {
            long double left, right, square_root, h = domain.indent.y + domain.origin.y + domain.size.y - obstacle.center.y;
            square_root = sqrt(std::pow(obstacle.radius, 2) - std::pow(h, 2));
            left = obstacle.center.x - square_root;
            right = obstacle.center.x + square_root;
            area = areaIntegral(obstacle, right_bound) - areaIntegral(obstacle, left_bound);
            area += areaIntegral(obstacle, left) - areaIntegral(obstacle, left_bound);
            area += areaIntegral(obstacle, right_bound) - areaIntegral(obstacle, right);
            area += square_root * 2 * h;
        } else {
            // shit happens
            std::cout << "WARN: obstacle is too big, actually obstacle takes all possible density." << ' ' << touches_bottom << ' ' << touches_top << std::endl;
        }
    }
    // std::cout << area << std::endl;
    return area;
}


inline long double getVolume(Assistant assistant, Domain domain, Obstacle obstacle) {
    return 0.0;
}


inline long double calculateDensity(Assistant assistant, Domain domain, Obstacle obstalce) {
    if (assistant.dimension == 2) {
        return getArea(assistant, domain, obstalce);
    } else if (assistant.dimension == 3) {
        return getVolume(assistant, domain, obstalce);
    } else {
        // raise error
        throw std::invalid_argument("ERROR: assistant.dimension is wrong!");
    }
}
