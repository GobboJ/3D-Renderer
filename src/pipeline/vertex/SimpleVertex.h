//
// Created by Jonathan on 09/04/2021.
//

#ifndef INC_3D_RENDERER_SIMPLEVERTEX_H
#define INC_3D_RENDERER_SIMPLEVERTEX_H

#include <array>
#include "../util/Vector3.h"

class SimpleVertex {
private:
    double x;
    double y;
    double z;
    unsigned int color;

public:
    SimpleVertex(double x, double y, double z, unsigned int color);

    double getX() const;

    double getY() const;

    double getZ() const;

    unsigned int getColor() const;

    void transform(const std::array<double, 16> &matrix);

    double project(const std::array<double, 16> &projectionMatrix);

    void viewportMapping(const std::array<double, 16> &viewportMatrix);

    static SimpleVertex
    interpolate(const SimpleVertex &v1, const SimpleVertex &v2, const SimpleVertex &v3, double A1,
                double A2, double A3, double w0, double w1, double w2);

private:
    inline double multiply(const std::array<double, 16> &matrix, int index) const;
};

#endif //INC_3D_RENDERER_SIMPLEVERTEX_H
