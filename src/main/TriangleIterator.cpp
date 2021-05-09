//
// Created by Jonathan on 09/05/2021.
//
#include "TriangleIterator.h"

TriangleIterator::TriangleIterator(const std::vector<SimpleVertex> &triangles, const std::vector<std::array<int, 3>> &indices,
                                   bool isLast)
        : triangles() {
    for (std::array<int, 3> i : indices) {
        this->triangles.emplace_back(
                std::array<SimpleVertex, 3>{triangles[i[0]], triangles[i[1]], triangles[i[2]]});
    }
    next = (isLast) ? this->triangles.size() : 0;
}

TriangleIterator::reference TriangleIterator::operator*() { return triangles.at(next); }

TriangleIterator::pointer TriangleIterator::operator->() { return &(triangles.at(next)); }

bool TriangleIterator::operator==(const TriangleIterator &other) const { return next == other.next; }

bool TriangleIterator::operator!=(const TriangleIterator &other) const { return next != other.next; }

// Postfix increment
TriangleIterator TriangleIterator::operator++(int) {

    TriangleIterator tmp = *this;
    ++(*this);
    return tmp;
}

// Prefix increment
TriangleIterator &TriangleIterator::operator++() {
    next++;
    return *this;
}
