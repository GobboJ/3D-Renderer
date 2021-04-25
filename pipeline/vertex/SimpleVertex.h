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

public:
    SimpleVertex(double x, double y, double z);

    double getX() const;

    double getY() const;

    double getZ() const;

    const SimpleVertex transform(std::array<double, 16> matrix) const;

private:
    inline double multiply(std::array<double, 16> matrix, int index) const;
};

#endif //INC_3D_RENDERER_SIMPLEVERTEX_H
