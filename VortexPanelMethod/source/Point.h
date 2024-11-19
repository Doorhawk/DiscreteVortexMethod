#pragma once
#include <stdexcept>
struct point
{
public:
    double x;
    double y;
    double abs() {
        return sqrt(x * x + y * y);
    }
    point() : x(0), y(0) {} 
    point(double x, double y) : x(x), y(y) {}
    point(const point& other) : x(other.x), y(other.y) {} 
    point& operator=(const point& other) {
        if (this != &other) { 
            x = other.x;
            y = other.y;
        }
        return *this;
    }
    point operator+(const point& other) const {
        return point(x + other.x, y + other.y);
    }
    point operator-(const point& other) const {
        return point(x - other.x, y - other.y);
    }
    point operator*(double scalar) const {
        return point(x * scalar, y * scalar);
    }
    point operator/(double scalar) const {
        if (scalar != 0) {
            return point(x / scalar, y / scalar);
        }
        else {
            throw std::invalid_argument("Division by zero.");
        }
    }
};