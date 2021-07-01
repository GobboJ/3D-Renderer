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


/**
 * Describes a simple type of mesh
 * @tparam V The type of the vertices
 */
template<class V>
class SimpleMesh {

private:

    // Vertices of the mesh
    std::vector<V> vertices;
    // Indices triplets of the vertices that make up each triangle
    std::vector<std::array<int, 3>> indices;

public:

    /**
     * Constructor of SimpleMesh
     * @param vertices The vertices of the mesh
     * @param indices The indices triplets that make up triangle
     */
    SimpleMesh(const std::vector<V> &vertices, const std::vector<std::array<int, 3>> &indices) : vertices(
            vertices), indices(indices) {}

    /**
     * Returns an iterator
     * @return
     */
    TriangleIterator<V> begin() const { return TriangleIterator<V>(vertices, indices, false); }

    TriangleIterator<V> end() const { return TriangleIterator<V>(vertices, indices, true); }

    const std::vector<V> &getVertices() const {
        return vertices;
    }
};

#endif //INC_3D_RENDERER_SIMPLEMESH_H
