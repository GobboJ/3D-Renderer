//
// Created by Jonathan on 06/05/2021.
//
#include "pipeline/Scene.h"
#include "pipeline/Camera.h"
#include "pipeline/vertex/SimpleVertex.h"
#include "pipeline/Pipeline.h"
#include <iterator>
#include <array>
#include <cstddef>
#include <iostream>
#include <string>

#define WIDTH 640
#define HEIGHT 480

class TriangleIterator {
private:
    std::vector<std::array<SimpleVertex, 3>> triangles;
    size_t next;


public:
    // Previously provided by std::iterator - see update below
    typedef std::array<SimpleVertex, 3> value_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type *pointer;
    typedef value_type &reference;
    typedef std::forward_iterator_tag iterator_category;

    TriangleIterator(const std::vector<SimpleVertex> &triangles, const std::vector<std::array<int,3>> &indices, bool isLast)
            : triangles() {
        for (std::array<int, 3> i : indices) {
            this->triangles.emplace_back(std::array<SimpleVertex, 3>{triangles[i[0]], triangles[i[1]], triangles[i[2]]});
        }
        next = (isLast) ? this->triangles.size() : 0;
    }

    reference operator*() { return triangles.at(next); }

    pointer operator->() { return &(triangles.at(next)); }

    bool operator==(const TriangleIterator &other) const { return next == other.next; }

    bool operator!=(const TriangleIterator &other) const { return next != other.next; }

    // Postfix increment
    TriangleIterator operator++(int) {

        TriangleIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // Prefix increment
    TriangleIterator &operator++() {
        next++;
        return *this;
    }
};

class SimpleMesh {
    std::vector<SimpleVertex> vertices;
    std::vector<std::array<int, 3>> indices;
public:
    SimpleMesh(const std::vector<SimpleVertex> &vertices, const std::vector<std::array<int, 3>> &indices) : vertices(
            vertices), indices(indices) {}

    TriangleIterator begin() const { return TriangleIterator(vertices, indices, false); }

    TriangleIterator end() const { return TriangleIterator(vertices, indices, true); }
};

struct SimpleShader {
    char operator()(SimpleVertex vertex) {

       // double z = fmod(vertex.getZ(), ((int) vertex.getZ()))*10;
        //return '0' + ((int) z);
        return std::to_string(vertex.getZ())[2];
    }
};

struct ColorShader {
    int operator()(SimpleVertex vertex) {

        // double z = fmod(vertex.getZ(), ((int) vertex.getZ()))*10;
        //return '0' + ((int) z);
        return 0;
    }
};


int main(int argc, char *argv[]) {

    const SimpleMesh cube({{1,  -1, 1.5},
                           {1,  1,  1.1},
                           {-1, 1,  1.5},
                           {-1, -1, 1.9}}, {{0, 1, 2}, {0, 2, 3}});

    const SimpleMesh rectangle({{0, 0, 0},
                                {4, 0, 0},
                                {0, 2, 0},
                                {4, 2, 0}}, {{0, 1, 2}, {1, 3, 2}});

    const Camera camera(45.0, 0.1, 10, {1.5, 1.5, 0}, {1.5, 1.5, -16});

    SimpleShader shader{};
    Object<SimpleMesh, SimpleVertex, SimpleShader> o(rectangle, shader);
    o.setPosition(0, 0, -6);
    o.setScale(1, 1, 1);
    o.setRotation(0, 0, 0);

    Scene<char> s(camera);
    s.add(o);

    char target[WIDTH * HEIGHT];
    memset(target, '.', WIDTH * HEIGHT);
    Pipeline<char> p(target, WIDTH, HEIGHT);

    p.render(s);
    for (std::array<SimpleVertex, 3> triangle : rectangle) {
        printf("(%.1f %.1f %.1f) (%.1f %.1f %.1f) (%.1f %.1f %.1f)\n", triangle[0].getX(), triangle[0].getY(),
               triangle[0].getZ(),
               triangle[1].getX(), triangle[1].getY(), triangle[1].getZ(),
               triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
    }

    // Prints target
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            std::cout << target[r * WIDTH + c];
        }
        std::cout << '\n';
    }

    return 0;
}
