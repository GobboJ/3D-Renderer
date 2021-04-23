//
// Created by Jonathan on 09/04/2021.
//

#include "SimpleVertex.h"

SimpleVertex::SimpleVertex(double x, double y, double z) : x(x), y(y), z(z) {}

double SimpleVertex::getX() const {
    return x;
}

double SimpleVertex::getY() const {
    return y;
}

double SimpleVertex::getZ() const {
    return z;
}

void SimpleVertex::transform(std::array<double, 16> matrix) {
    x = matrix[0] * x + matrix[1] * y + matrix[2] * z + matrix[3];
    y = matrix[4] * x + matrix[5] * y + matrix[6] * z + matrix[7];
    z = matrix[8] * x + matrix[9] * y + matrix[10] * z + matrix[11];
}
