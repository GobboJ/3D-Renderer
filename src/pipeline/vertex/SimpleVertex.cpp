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

inline double SimpleVertex::multiply(const std::array<double, 16> &matrix, const int index) const {

    return matrix[index] * x + matrix[index + 1] * y + matrix[index + 2] * z +
           matrix[index + 3];
}

void SimpleVertex::transform(const std::array<double, 16> &matrix) {
    double new_w = multiply(matrix, 12);
    x = multiply(matrix, 0) / new_w;
    y = multiply(matrix, 4) / new_w;
    z = multiply(matrix, 8) / new_w;
}

double SimpleVertex::project(const std::array<double, 16> &projectionMatrix) {
    double w = multiply(projectionMatrix, 12);
    x = multiply(projectionMatrix, 0) / w;
    y = multiply(projectionMatrix, 4) / w;
    z = multiply(projectionMatrix, 8) / w;
    return w;
}

void SimpleVertex::viewportMapping(const std::array<double, 16> &viewportMatrix) {
    x = x * viewportMatrix[0] + viewportMatrix[3];
    y = y * viewportMatrix[5] + viewportMatrix[7];
    z = z * viewportMatrix[10] + viewportMatrix[11];
}

SimpleVertex
SimpleVertex::interpolate(const SimpleVertex &v1, const SimpleVertex &v2, const SimpleVertex &v3, const double A1,
                          const double A2, const double A3, const double w0, const double w1, const double w2) {
    const double sum = A1 + A2 + A3;

    Vector3 v1Pos = {v1.getX(), v1.getY(), v1.getZ()};
    Vector3 v2Pos = {v2.getX(), v2.getY(), v3.getZ()};
    Vector3 v3Pos = {v3.getX(), v3.getY(), v3.getZ()};

    Vector3 pos = (A1 / sum) * v1Pos + (A2 / sum) * v2Pos + (A3 / sum) * v3Pos;

    /*Vector3 norm = ((A1 / sum) / pos.getZ() * v1.getNorm() + (A2 / sum) / pos.getZ() * v2.getNorm() +
                    (A3 / sum) / pos.getZ() * v3.getNorm()) /
                   ((A1 / sum) / pos.getZ() + (A2 / sum) / pos.getZ() + (A3 / sum) / pos.getZ());
    Vector2 uv = ((A1 / sum) / pos.getZ() * v1.getUv() + (A2 / sum) / pos.getZ() * v2.getUv() +
                  (A3 / sum) / pos.getZ() * v3.getUv()) /
                 ((A1 / sum) / pos.getZ() + (A2 / sum) / pos.getZ() + (A3 / sum) / pos.getZ());*/

    return {pos.getX(), pos.getY(), pos.getZ()};
}
