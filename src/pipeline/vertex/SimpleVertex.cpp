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

SimpleVertex
SimpleVertex::interpolate(const SimpleVertex &v1, const SimpleVertex &v2, const SimpleVertex &v3, const double A1,
                          const double A2, const double A3, const double w0, const double w1, const double w2) {
    const double sum = A1 + A2 + A3;

    Vector3 v1Pos = {v1.getX(), v1.getY(), v1.getZ()};
    Vector3 v2Pos = {v2.getX(), v2.getY(), v2.getZ()};
    Vector3 v3Pos = {v3.getX(), v3.getY(), v3.getZ()};

    // Computes the interpolated position
    Vector3 pos = (A1 / sum) * v1Pos + (A2 / sum) * v2Pos + (A3 / sum) * v3Pos;

    // Computes the correct z (could be useless)
    //double z_correction = (A1 / sum) * v1.getZ() + (A2 / sum) * v2.getZ() + (A3 / sum) * v3.getZ();

    // Interpolates the color

    double a = (A1 / sum);
    double b = (A2 / sum);
    double c = (A3 / sum);

    auto rV1 = static_cast<unsigned char> ((v1.getColor() >> 16) & 0xFF);
    auto gV1 = static_cast<unsigned char> ((v1.getColor() >> 8) & 0xFF);
    auto bV1 = static_cast<unsigned char> (v1.getColor() & 0xFF);

    auto rV2 = static_cast<unsigned char> ((v2.getColor() >> 16) & 0xFF);
    auto gV2 = static_cast<unsigned char> ((v2.getColor() >> 8) & 0xFF);
    auto bV2 = static_cast<unsigned char> (v2.getColor() & 0xFF);

    auto rV3 = static_cast<unsigned char> ((v3.getColor() >> 16) & 0xFF);
    auto gV3 = static_cast<unsigned char> ((v3.getColor() >> 8) & 0xFF);
    auto bV3 = static_cast<unsigned char> (v3.getColor() & 0xFF);

    unsigned int redColor =
            (a * rV1 / w0 + b * rV2 / w1 + c * rV3 / w2) / // NOLINT(cppcoreguidelines-narrowing-conversions)
            (a / w0 + b / w1 + c / w2);
    unsigned int greenColor =
            (a * gV1 / w0 + b * gV2 / w1 + c * gV3 / w2) / // NOLINT(cppcoreguidelines-narrowing-conversions)
            (a / w0 + b / w1 + c / w2);
    unsigned int blueColor =
            (a * bV1 / w0 + b * bV2 / w1 + c * bV3 / w2) / // NOLINT(cppcoreguidelines-narrowing-conversions)
            (a / w0 + b / w1 + c / w2);

    unsigned int color = (redColor << 16) | (greenColor << 8) | blueColor;

    // Creates the interpolated vertex
    SimpleVertex v = {pos.getX(), pos.getY(), pos.getZ(), color};
    return v;
}

unsigned int SimpleVertex::getColor() const {
    return color;
}

double SimpleVertex::distance(SimpleVertex &vertex) const {
    return sqrt(pow(vertex.getX() - this->x, 2) + pow(vertex.getY() - this->y, 2) + pow(vertex.getZ() - this->z, 2));
}
