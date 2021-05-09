//
// Created by Jonathan on 09/05/2021.
//

#ifndef INC_3D_RENDERER_TRIANGLEITERATOR_H
#define INC_3D_RENDERER_TRIANGLEITERATOR_H

#include <cstddef>
#include <array>
#include <iterator>
#include "../pipeline/Pipeline.h"
#include "../pipeline/vertex/SimpleVertex.h"
#include "../pipeline/Camera.h"

class TriangleIterator {
private:
    std::vector<std::array<SimpleVertex, 3>> triangles;
    size_t next;

public:
    // Previously provided by std::iterator - see update below
    typedef std::array<SimpleVertex, 3> value_type;
    typedef ptrdiff_t difference_type;
    typedef value_type *pointer;
    typedef value_type &reference;
    typedef std::forward_iterator_tag iterator_category;

    TriangleIterator(const std::vector<SimpleVertex> &triangles, const std::vector<std::array<int, 3>> &indices,
                     bool isLast);

    reference operator*();

    pointer operator->();

    bool operator==(const TriangleIterator &other) const;

    bool operator!=(const TriangleIterator &other) const;

    // Postfix increment
    TriangleIterator operator++(int);

    // Prefix increment
    TriangleIterator &operator++();
};
#endif //INC_3D_RENDERER_TRIANGLEITERATOR_H
