//
// Created by Jonathan on 09/05/2021.
//

#include "SimpleMesh.h"

SimpleMesh::SimpleMesh(const std::vector<SimpleVertex> &vertices, const std::vector<std::array<int, 3>> &indices) : vertices(
        vertices), indices(indices) {}

TriangleIterator SimpleMesh::begin() const { return TriangleIterator(vertices, indices, false); }

TriangleIterator SimpleMesh::end() const { return TriangleIterator(vertices, indices, true); }