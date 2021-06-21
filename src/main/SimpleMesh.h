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

template<class V>
class SimpleMesh {
    std::vector<V> vertices;
    std::vector<std::array<int, 3>> indices;
public:
    SimpleMesh(const std::vector<V> &vertices, const std::vector<std::array<int, 3>> &indices) : vertices(
            vertices), indices(indices) {}

    TriangleIterator<V> begin() const { return TriangleIterator<V>(vertices, indices, false); }

    TriangleIterator<V> end() const { return TriangleIterator<V>(vertices, indices, true); }

    const std::vector<V> &getVertices() const {
        return vertices;
    }
};

#endif //INC_3D_RENDERER_SIMPLEMESH_H
