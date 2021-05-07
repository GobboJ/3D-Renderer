//
// Created by Jonathan on 16/04/2021.
//

#include "Vector3.h"

double Vector3::getX() const {
    return x;
}

double Vector3::getY() const {
    return y;
}

double Vector3::getZ() const {
    return z;
}

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
