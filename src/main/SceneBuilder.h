//
// Created by Jonathan on 09/05/2021.
//

#ifndef INC_3D_RENDERER_SCENEBUILDER_H
#define INC_3D_RENDERER_SCENEBUILDER_H

#include "../pipeline/Scene.h"
#include "../pipeline/Camera.h"
#include "../pipeline/vertex/SimpleVertex.h"
#include "../pipeline/Pipeline.h"
#include "SimpleMesh.h"
#include <iterator>
#include <array>
#include <cstddef>
#include <iostream>
#include <string>

template<class T, class S>
Scene<T> createScene(S &shader, int frame = 0) {
    const SimpleMesh rectangle({{0, 0, 0, 0xFF0000},
                                {4, 0, 0, 0x00FF00},
                                {0, 2, 0, 0x0000FF},
                                {4, 2, 0, 0xFFFFFF}}, {/*{0, 1, 2},*/
                                                       {1, 3, 2}});

    const SimpleMesh cube({{1,  1,  -1, 0xFF0000},
                           {1,  -1, -1, 0xFFFF00},
                           {1,  1,  1,  0xFF00FF},
                           {1,  -1, 1,  0x0000FF},
                           {-1, 1,  -1, 0x00FFFF},
                           {-1, -1, -1, 0x00FF00},
                           {-1, 1,  1,  0x80FF00},
                           {-1, -1, 1,  0x808080}},
                          {{4, 2, 0},
                           {2, 7, 3},
                           {6, 5, 7},
                           {1, 7, 5},
                           {0, 3, 1},
                           {4, 1, 5},
                           {4, 6, 2},
                           {2, 6, 7},
                           {6, 4, 5},
                           {1, 3, 7},
                           {0, 2, 3},
                           {4, 0, 1}});


    const Camera camera(45.0, 0.1, 10, {0, 0, 0}, {0, 0, -3.5});


    Object<SimpleMesh, SimpleVertex, S> o(cube, shader);
    o.setPosition(0, 0, -3.5);
    o.setScale(1, 1, 1);
    o.setRotation(0, frame%360, 0);

    Object<SimpleMesh, SimpleVertex, S> o2(rectangle, shader);
    o2.setPosition(0,0, -7);
    o2.setRotation(0, 150, 0);
    Scene<T> s(camera);
    //s.add(o2);
    s.add(o);
    return s;
}


#endif //INC_3D_RENDERER_SCENEBUILDER_H
