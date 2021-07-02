//
// Created by Jonathan on 10/05/2021.
//

#include "TextureVertex.h"
#include "../util/Vector3.h"


double TextureVertex::getX() const {
    return x;
}

double TextureVertex::getY() const {
    return y;
}

double TextureVertex::getZ() const {
    return z;
}


inline double TextureVertex::multiply(const std::array<double, 16> &matrix, const int index) const {

    return matrix[index] * x + matrix[index + 1] * y + matrix[index + 2] * z +
           matrix[index + 3];
}

void TextureVertex::transform(const std::array<double, 16> &matrix) {
    double new_w = multiply(matrix, 12);
    double new_x = multiply(matrix, 0) / new_w;
    double new_y = multiply(matrix, 4) / new_w;
    double new_z = multiply(matrix, 8) / new_w;

    x = new_x;
    y = new_y;
    z = new_z;
}

double TextureVertex::project(const std::array<double, 16> &projectionMatrix) {
    double new_w = multiply(projectionMatrix, 12);
    double new_x = multiply(projectionMatrix, 0) / new_w;
    double new_y = multiply(projectionMatrix, 4) / new_w;
    double new_z = multiply(projectionMatrix, 8) / new_w;

    x = new_x;
    y = new_y;
    z = new_z;
    return new_w;
}

void TextureVertex::viewportMapping(const std::array<double, 16> &viewportMatrix) {
    x = x * viewportMatrix[0] + viewportMatrix[3];
    y = y * viewportMatrix[5] + viewportMatrix[7];
    z = z * viewportMatrix[10] + viewportMatrix[11];
}

TextureVertex
TextureVertex::interpolate(const TextureVertex &v1, const TextureVertex &v2, const TextureVertex &v3, const double A1,
                           const double A2, const double A3, const double w0, const double w1, const double w2) {



    // Computes the interpolated position
    Vector3 pos{v1.getX() * A1 + v2.getX() * A2 + v3.getX() * A3,
                v1.getY() * A1 + v2.getY() * A2 + v3.getY() * A3,
                v1.getZ() * A1 + v2.getZ() * A2 + v3.getZ() * A3};

    // Computes the correct z (could be useless)
    // double z_correction = (A1 / sum) * v1.getZ() + (A2 / sum) * v2.getZ() + (A3 / sum) * v3.getZ();

    double invW0 = A1 / w0;
    double invW1 = A2 / w1;
    double invW2 = A3 / w2;
    double sum = invW0 + invW1 + invW2;

    // Computes the interpolated texture coordinates
    double int_u = (invW0 * v1.getU() + invW1 * v2.getU() + invW2 * v3.getU()) / sum;
    double int_v = (invW0 * v1.getV() + invW1 * v2.getV() + invW2 * v3.getV()) / sum;

    // Creates the interpolated vertex
    TextureVertex v = {pos.getX(), pos.getY(), pos.getZ(), int_u, int_v};
    return v;
}

TextureVertex::TextureVertex(double x, double y, double z, double u, double v) : x(x), y(y), z(z), u(u), v(v) {}

double TextureVertex::getU() const {
    return u;
}

double TextureVertex::getV() const {
    return v;
}

double TextureVertex::distance(TextureVertex &vertex) const {
    return sqrt(pow(vertex.getX() - this->x, 2) + pow(vertex.getY() - this->y, 2) + pow(vertex.getZ() - this->z, 2));
}
