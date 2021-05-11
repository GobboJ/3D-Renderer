//
// Created by Jonathan on 09/05/2021.
//

#ifndef INC_3D_RENDERER_TRIANGLEITERATOR_H
#define INC_3D_RENDERER_TRIANGLEITERATOR_H

#include <cstddef>
#include <array>
#include <iterator>
#include "../pipeline/Pipeline.h"
#include "../pipeline/Camera.h"

template<class V>
class TriangleIterator {
private:
    std::vector<std::array<V, 3>> triangles;
    size_t next;

public:
    // Previously provided by std::iterator - see update below
    typedef std::array<V, 3> value_type;
    typedef ptrdiff_t difference_type;
    typedef value_type *pointer;
    typedef value_type &reference;
    typedef std::forward_iterator_tag iterator_category;

    TriangleIterator(const std::vector<V> &triangles, const std::vector<std::array<int, 3>> &indices,
                     bool isLast)
            : triangles() {
        for (std::array<int, 3> i : indices) {
            this->triangles.emplace_back(
                    std::array<V, 3>{triangles[i[0]], triangles[i[1]], triangles[i[2]]});
        }
        next = (isLast) ? this->triangles.size() : 0;
    }

    reference operator*(){ return triangles.at(next); }

    pointer operator->(){ return &(triangles.at(next)); }

    bool operator==(const TriangleIterator<V> &other) const { return next == other.next; }

    bool operator!=(const TriangleIterator<V> &other) const { return next != other.next; }

    // Postfix increment
    TriangleIterator<V> operator++(int){

        TriangleIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // Prefix increment
    TriangleIterator<V> &operator++(){
        next++;
        return *this;
    }
};
#endif //INC_3D_RENDERER_TRIANGLEITERATOR_H
