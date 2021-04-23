#include <iostream>
#include <vector>
#include "pipeline/vertex/SimpleVertex.h"
#include "pipeline/object/Object.h"
#include "pipeline/Camera.h"

char shader() {
    return 'a';
}

double shader2() {
    return 1.0f;
}

int main() {

    // Tests

    SimpleVertex v1(0, 0, 0);
    SimpleVertex v2(1, 1, 1);
    SimpleVertex v3(2, 2, 2);

    Camera c(45.0f, 1, 2, {0, 0, 0}, {0, 0, -10});

    // Objects tests
    Object o1(std::vector<SimpleVertex>({v1, v2, v3}), &shader);
    Object o2(std::vector<SimpleVertex>({v2, v1, v3}), &shader2);
    std::vector<Object> objs;
    objs.push_back(std::move(o1));
    objs.push_back(std::move(o2));
    for (auto &o : objs) {

    }
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