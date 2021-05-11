//
// Created by Jonathan on 09/05/2021.
//

#ifndef INC_3D_RENDERER_SHADERS_H
#define INC_3D_RENDERER_SHADERS_H

#include "../pipeline/Scene.h"
#include "../pipeline/Camera.h"
#include "../pipeline/vertex/SimpleVertex.h"
#include "../pipeline/Pipeline.h"
#include <iterator>
#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <cmath>

struct CharShader {
    char operator()(SimpleVertex vertex, char texture) {

        return static_cast<char>((abs(vertex.getZ())) * 10.0 + 0.5) % 10 + '0';
        //return std::to_string(vertex.getZ())[3];
    }
};

struct ColorShader {
    uint32_t operator()(SimpleVertex vertex) {

        return vertex.getColor();
    }
};


#endif //INC_3D_RENDERER_SHADERS_H
