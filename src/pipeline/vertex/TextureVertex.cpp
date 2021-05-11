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

    Vector3 v1Pos = {v1.getX(), v1.getY(), v1.getZ()};
    Vector3 v2Pos = {v2.getX(), v2.getY(), v2.getZ()};
    Vector3 v3Pos = {v3.getX(), v3.getY(), v3.getZ()};

    Vector3 pos = (A1) * v1Pos + (A2) * v2Pos + (A3) * v3Pos;
    /*double z_correction =
            (A1) * v1.getZ() + (A2) * v2.getZ() + (A3) * v3.getZ(); // ?? E' sempre uguale a pos.z
*/
    double a = A1;
    double b = A2;
    double c = A3;

    double int_u = (a * v1.getU() / w0 + b * v2.getU() / w1 + c * v3.getU() / w2) / (a / w0 + b / w1 + c / w2);
    double int_v = (a * v1.getV() / w0 + b * v2.getV() / w1 + c * v3.getV() / w2) / (a / w0 + b / w1 + c / w2);

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
