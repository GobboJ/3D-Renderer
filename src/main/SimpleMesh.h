//
// Created by Jonathan on 09/05/2021.
//

#ifndef INC_3D_RENDERER_SIMPLEMESH_H
#define INC_3D_RENDERER_SIMPLEMESH_H

#include "../pipeline/Scene.h"
#include "../pipeline/Camera.h"
#include "../pipeline/vertex/SimpleVertex.h"
#include "../pipeline/Pipeline.h"
#include "TriangleIterator.h"
#include <iterator>
#include <array>
#include <cstddef>
#include <iostream>
#include <string>

class SimpleMesh {
    std::vector<SimpleVertex> vertices;
    std::vector<std::array<int, 3>> indices;
public:
    SimpleMesh(const std::vector<SimpleVertex> &vertices, const std::vector<std::array<int, 3>> &indices);

    TriangleIterator begin() const;

    TriangleIterator end() const;
};


#endif //INC_3D_RENDERER_SIMPLEMESH_H