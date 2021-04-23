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

    void transform(std::array<double, 16> matrix);
};

#endif //INC_3D_RENDERER_SIMPLEVERTEX_H
