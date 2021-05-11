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

template<class T, class S, class Texture>
Scene<T> createSimpleScene(S &shader, Texture texture, int frame = 0) {
    const SimpleMesh<SimpleVertex> rectangle({{0, 0, 0, 0xFF0000},
                                {4, 0, 0, 0x00FF00},
                                {0, 2, 0, 0x0000FF},
                                {4, 2, 0, 0xFFFFFF}}, {{0, 1, 2},
                                                       {1, 3, 2}});

    const SimpleMesh<SimpleVertex> cube({{1,  1,  -1, 0xFF0000},
                           {1,  -1, -1, 0xFFFF00},
                           {1,  1,  1,  0xFF00FF},
                           {1,  -1, 1,  0x0000FF},
                           {-1, 1,  -1, 0x00FFFF},
                           {-1, -1, -1, 0x00FF00},
                           {-1, 1,  1,  0x80FF00},
                           {-1, -1, 1,  0x808080}},
                          {
                                  // Top
                                  {4, 2, 0},
                                  {4, 6, 2},
                                  // Bottom
                                  {1, 3, 7},
                                  {1, 7, 5},
                                  // Front
                                  {2, 7, 3},
                                  {2, 6, 7},
                                  // Left
                                  {6, 5, 7},
                                  {6, 4, 5},
                                  // Right
                                  {0, 3, 1},
                                  {0, 2, 3},
                                  // Back
                                  {4, 1, 5},
                                  {4, 0, 1}
                          });


    const Camera camera(45.0, 0.1, 10, {0, 0, 0}, {0, 0, -3.5});


    Object<SimpleMesh<SimpleVertex>, SimpleVertex, S, Texture> o(cube, shader, texture);
    o.setPosition(0, 0, -3.5);
    o.setScale(1, 1, 1);
    o.setRotation(0, frame%360, 0);

    Object<SimpleMesh<SimpleVertex>, SimpleVertex, S, Texture> o2(rectangle, shader, texture);
    o2.setPosition(0, 0, -7);
    o2.setRotation(0, 0, 0);
    Scene<T> s(camera);
    //s.add(o2);
    s.add(o);
    return s;
}

template<class T, class S, class Texture>
Scene<T> createTextureScene(S &shader, Texture &texture, int frame = 0) {
    const SimpleMesh<TextureVertex> rectangle({{0, 0, 0, 0, 0},
                                              {4, 0, 0, 0, 1},
                                              {0, 2, 0, 1, 0},
                                              {4, 2, 0, 1, 1}}, {{0, 1, 2},
                                                                     {1, 3, 2}});

    const SimpleMesh<TextureVertex> cube({{1,  1,  -1, 1, 1}, // BUR
                                         {1,  -1, -1, 0, 0}, // BDR
                                         {1,  1,  1,  0, 1}, // FUR
                                         {1,  -1, 1,  1, 0}, // FDR
                                         {-1, 1,  -1, 1, 0}, // BUL
                                         {-1, -1, -1, 0, 1}, // BDL
                                         {-1, 1,  1,  0, 0}, // FUL
                                         {-1, -1, 1,  1, 1}}, // FDL
                                        {
                                                // Top
                                                {4, 2, 0},
                                                {4, 6, 2},
                                                // Bottom
                                                {1, 3, 7},
                                                {1, 7, 5},
                                                // Front
                                                {2, 7, 3},
                                                {2, 6, 7},
                                                // Left
                                                {6, 5, 7},
                                                {6, 4, 5},
                                                // Right
                                                {0, 3, 1},
                                                {0, 2, 3},
                                                // Back
                                                {4, 1, 5},
                                                {4, 0, 1}
                                        });

    const SimpleMesh<TextureVertex> newCube({{0,0,0,0,0}, {0,0,0,1,1}, {0,0,0,1,0},// 0 - 2
                                               {0,0,1,1,0}, {0,0,1,0,1}, {0,0,1,0,0},// 3 - 5
                                               {0,1,0,0,1}, {0,1,0,1,1}, {0,1,0,1,1},// 6 - 8
                                               {0,1,1,1,1}, {0,1,1,0,1}, {0,1,1,0,1},// 9 - 11
                                               {1,0,0,1,0}, {1,0,0,0,0}, {1,0,0,1,0},// 12 - 14
                                               {1,0,1,0,0}, {1,0,1,0,0}, {1,0,1,1,0},// 15 - 17
                                               {1,1,0,0,1}, {1,1,0,1,1}, {1,1,0,1,0},// 18 - 20
                                               {1,1,1,0,1}, {1,1,1,0,0}, {1,1,1,1,1}},// 21 - 23
                                              {{0,3,6}, {3,6,9}, {1,4,12}, {4, 12,15},
                                               {2,7,13}, {7,13,18}, {8,10,20}, {10,20,22},
                                               {14,16,19}, {16,19,21}, {5,11,17}, {11,17,23}});


    const Camera camera(45.0, 0.1, 10, {0, 0, 0}, {0, 0, -3.5});


    Object<SimpleMesh<TextureVertex>, TextureVertex, S, Texture> o(newCube, shader, texture);
    o.setPosition(0, 0, -3);
    o.setScale(1, 1, 1);
    o.setRotation(frame%360, frame%360, frame%360);

    Object<SimpleMesh<TextureVertex>, TextureVertex, S, Texture> o2(rectangle, shader, texture);
    o2.setPosition(0, 0, -7);
    o2.setRotation(0, 0, 0);
    Scene<T> s(camera);
    //s.add(o2);
    s.add(o);
    return s;
}

#endif //INC_3D_RENDERER_SCENEBUILDER_H
