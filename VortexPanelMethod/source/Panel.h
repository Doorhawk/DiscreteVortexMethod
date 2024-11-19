#pragma once
#include "point.h"
#include <cmath>

class Panel {
public:
	point start;
	point end;
	point control; //no-penetration point
	point vortex;
	point norm;
	double intensity;
    Panel() : start(point()), end(point()), control(point()), vortex(point()), norm(point()), intensity(0) {}
    Panel(const point& _start, const point& _end) : start(_start), end(_end), intensity(0) {
        control = (start + end) / 2.0;
        vortex = start;

        norm = { sin(atan2((end.y - start.y), (end.x - start.x))),
                -cos(atan2((end.y - start.y), (end.x - start.x))) };
    }
    Panel(const Panel& other)
        : start(other.start), end(other.end), control(other.control), vortex(other.vortex),
        norm(other.norm), intensity(other.intensity) {}
    Panel& operator=(const Panel& other) {
        if (this != &other) {
            start = other.start;
            end = other.end;
            control = other.control;
            vortex = other.vortex;
            norm = other.norm;
            intensity = other.intensity;
        }
        return *this;
    }
};