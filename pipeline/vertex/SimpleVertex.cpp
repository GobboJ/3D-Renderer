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

SimpleVertex SimpleVertex::transform(std::array<double, 16> matrix) const {
    double new_x = multiply(matrix, 0);//[0] * x + matrix[1] * y + matrix[2] * z + matrix[3];
    double new_y = multiply(matrix, 4);//[4] * x + matrix[5] * y + matrix[6] * z + matrix[7];
    double new_z = multiply(matrix, 8);//[8] * x + matrix[9] * y + matrix[10] * z + matrix[11];
    double new_w = multiply(matrix, 12);//[12] * x + matrix[13] * y + matrix[14] * z + matrix [15];

    return {new_x/new_w, new_y/new_w, new_z/new_w};
}

inline double SimpleVertex::multiply(std::array<double, 16> matrix, const int index) const {

    return matrix[index] * x + matrix[index+1] * y + matrix[index+2] * z + matrix[index+3];
}