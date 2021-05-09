//
// Created by Jonathan on 09/04/2021.
//

#include "SimpleVertex.h"

SimpleVertex::SimpleVertex(double x, double y, double z, unsigned int color) : x(x), y(y), z(z), color(color) {}

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
    double new_x = multiply(matrix, 0) / new_w;
    double new_y = multiply(matrix, 4) / new_w;
    double new_z = multiply(matrix, 8) / new_w;

    x = new_x;
    y = new_y;
    z = new_z;
}

double SimpleVertex::project(const std::array<double, 16> &projectionMatrix) {
    double new_w = multiply(projectionMatrix, 12);
    double new_x = multiply(projectionMatrix, 0) / new_w;
    double new_y = multiply(projectionMatrix, 4) / new_w;
    double new_z = multiply(projectionMatrix, 8) / new_w;

    x = new_x;
    y = new_y;
    z = new_z;
    return new_w;
}

void SimpleVertex::viewportMapping(const std::array<double, 16> &viewportMatrix) {
    x = x * viewportMatrix[0] + viewportMatrix[3];
    y = y * viewportMatrix[5] + viewportMatrix[7];
    z = z * viewportMatrix[10] + viewportMatrix[11];
}

/*
SimpleVertex
SimpleVertex::interpolate(const SimpleVertex &v1, const SimpleVertex &v2, const SimpleVertex &v3, const double A1,
                          const double A2, const double A3, const double w0, const double w1, const double w2) {
    const double sum = A1 + A2 + A3;

    Vector3 v1Pos = {v1.getX(), v1.getY(), v1.getZ()};
    Vector3 v2Pos = {v2.getX(), v2.getY(), v3.getZ()};
    Vector3 v3Pos = {v3.getX(), v3.getY(), v3.getZ()};

    Vector3 pos = (A1 / sum) * v1Pos + (A2 / sum) * v2Pos + (A3 / sum) * v3Pos;

    *Vector3 norm = ((A1 / sum) / pos.getZ() * v1.getNorm() + (A2 / sum) / pos.getZ() * v2.getNorm() +
                    (A3 / sum) / pos.getZ() * v3.getNorm()) /
                   ((A1 / sum) / pos.getZ() + (A2 / sum) / pos.getZ() + (A3 / sum) / pos.getZ());
    Vector2 uv = ((A1 / sum) / pos.getZ() * v1.getUv() + (A2 / sum) / pos.getZ() * v2.getUv() +
                  (A3 / sum) / pos.getZ() * v3.getUv()) /
                 ((A1 / sum) / pos.getZ() + (A2 / sum) / pos.getZ() + (A3 / sum) / pos.getZ());*

    return {pos.getX(), pos.getY(), pos.getZ()};
}*/

SimpleVertex
SimpleVertex::interpolate(const SimpleVertex &v1, const SimpleVertex &v2, const SimpleVertex &v3, const double A1,
                          const double A2, const double A3, const double w0, const double w1, const double w2) {
    const double sum = A1 + A2 + A3;

    Vector3 v1Pos = {v1.getX(), v1.getY(), v1.getZ()};
    Vector3 v2Pos = {v2.getX(), v2.getY(), v3.getZ()};
    Vector3 v3Pos = {v3.getX(), v3.getY(), v3.getZ()};

    Vector3 pos = (A1 / sum) * v1Pos + (A2 / sum) * v2Pos + (A3 / sum) * v3Pos;

    bool wireframe = abs(A1) <= 100 || abs(A2) <= 100 || abs(A3) <= 100;
    unsigned int color = (v1.getColor() + v2.getColor() + v3.getColor()) / 3;
    SimpleVertex v = {pos.getX(), pos.getY(), pos.getZ(), color};
    v.wireframe = wireframe;

    return v;
}

bool SimpleVertex::isWireframe() const {
    return wireframe;
}

unsigned int SimpleVertex::getColor() const {
    return color;
}