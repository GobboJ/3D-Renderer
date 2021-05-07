#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include "pipeline/vertex/SimpleVertex.h"
#include "pipeline/object/Object.h"
#include "pipeline/Camera.h"
#include "pipeline/Scene.h"
#include "pipeline/Pipeline.h"

char shader() {
    return 'a';
}

double shader2() {
    return 1.0f;
}

struct Mesh {
    std::vector<SimpleVertex> vertices;
    std::vector<std::array<int, 3>> indices;

    Mesh(std::vector<SimpleVertex> vertices, std::vector<std::array<int, 3>> indices) : vertices(std::move(vertices)),
                                                                                        indices(std::move(indices)) {}
};

int main() {


    const Mesh cube({{1,  -1, 1.5},
               {1,  1,  1.1},
               {-1, 1,  1.5},
               {-1, -1, 1.9}}, {{0, 1, 2},
                                {0, 2, 3}});

    const Camera c(45.0f, 1, 10, {1, 0, 0}, {1, 0, -5});

    Object o1(cube, &shader);
    o1.setPosition(1, 0, -5);
    o1.setScale(1, 1, 1);
    o1.setRotation(0, 0, 0);
//    std::vector<Object> objs;
//    objs.push_back(std::move(o1));
//    objs.push_back(std::move(o2));
//    for (auto &o : objs) {
//
//    }

    // TODO: Da decidere rotazione in angoli o 3 vettori
    std::vector<Object> objs;
    objs.emplace_back(std::move(o1));

    Scene<char> s(objs, c);

    char *target = new char[150 * 50];
    Pipeline<char> pipeline(target, 150, 50);
    pipeline.render(s);
    /*for(auto &t : s.getObjects()) {
        //Object o = std::forward<Object>(std::get<0>(t));
        Vector3 x = std::get<1>(t);
        Vector3 y = std::get<2>(t);

        int a = 10;
    }*/
    return 0;
}



/*
 * Oggetto
 *  file obj coordinate
 *  file texture
 *  posizione, rotazione, scala
 *
 *  Dall'esterno passa gli oggetti originali const alla pipeline
 *
 */

/*
 * - pipeline
 *     - vertex
 *     - object
 *     - shader
 * - frontend
 * - lettura files
*/

/*
 * rvalue and forward reference
void aaaa(SimpleVertex&& v) {

}
void test(SimpleVertex&& v) {
    aaaa(v); // NON VA
    aaaa(std::forward<SimpleVertex>(v)); // VA
}
 SimpleVertex v1(0,0,0);
 test(v1); // NON VA
 test(SimpleVertex(1,2,3)); // VA
 */