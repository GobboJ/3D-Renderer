//
// Created by Jonathan on 01/07/2021.
//

#ifndef INC_3D_RENDERER_SAMPLESCENE_H
#define INC_3D_RENDERER_SAMPLESCENE_H


#include "../pipeline/object/Object.h"
#include "SimpleMesh.h"
#include "Shaders.h"

template<class target_t, class FirstShader, class SecondShader, class Texture>
class SampleScene {
private:
    const SimpleMesh<SimpleVertex> colorMesh = {{{1, 1, -1, 0xFF0000},
                                                        {1, -1, -1, 0xFFFF00},
                                                        {1, 1, 1, 0xFF00FF},
                                                        {1, -1, 1, 0x0000FF},
                                                        {-1, 1, -1, 0x00FFFF},
                                                        {-1, -1, -1, 0x00FF00},
                                                        {-1, 1, 1, 0x80FF00},
                                                        {-1, -1, 1, 0x808080}},
                                                {
                                                        // Top
                                                 {4, 2, 0},
                                                        {4, 6,  2},
                                                        // Bottom
                                                        {1, 3, 7},
                                                        {1, 7,  5},
                                                        // Front
                                                        {2,  7, 3},
                                                        {2,  6,  7},
                                                        // Left
                                                        {6,  5, 7},
                                                        {6,  4,  5},
                                                        // Right
                                                        {0, 3, 1},
                                                        {0, 2, 3},
                                                        // Back
                                                        {4, 1, 5},
                                                        {4, 0, 1}
                                                }};

    const SimpleMesh<TextureVertex> textureMesh = {{{0, 0, 0, 0, 0},
                                                           {0, 0, 0, 1, 1},
                                                           {0, 0, 0, 1, 0},// 0 - 2
                                                           {0, 0,  1, 1, 0},
                                                           {0, 0, 1, 0, 1},
                                                           {0, 0,  1, 0, 0},// 3 - 5
                                                           {0, 1,  0, 0, 1},
                                                           {0,  1,  0, 1, 1},
                                                           {0,  1,  0, 1, 1},// 6 - 8
                                                           {0,  1,  1, 1, 1},
                                                           {0, 1,  1, 0, 1},
                                                           {0,  1,  1, 0, 1},// 9 - 11
                                                           {1, 0, 0, 1, 0},
                                                           {1, 0, 0, 0, 0},
                                                           {1, 0, 0, 1, 0},// 12 - 14
                                                           {1, 0, 1, 0, 0},
                                                           {1, 0, 1, 0, 0},
                                                           {1, 0, 1, 1, 0},// 15 - 17
                                                           {1, 1, 0, 0, 1},
                                                           {1, 1, 0, 1, 1},
                                                           {1, 1, 0, 1, 0},// 18 - 20
                                                           {1, 1, 1, 0, 1},
                                                           {1, 1, 1, 0, 0},
                                                           {1, 1, 1, 1, 1}},// 21 - 23
                                                   {{0, 3, 6},
                                                           {3, 6, 9},
                                                           {1, 4, 12},
                                                           {4, 12, 15},
                                                           {2, 7, 13},
                                                           {7, 13, 18},
                                                           {8, 10, 20},
                                                           {10, 20, 22},
                                                           {14, 16, 19},
                                                           {16, 19, 21},
                                                           {5, 11, 17},
                                                           {11, 17, 23}}};

    Object<SimpleMesh<SimpleVertex>, SimpleVertex, FirstShader, Texture> colorCube;
    Object<SimpleMesh<TextureVertex>, TextureVertex, SecondShader, Texture> textureCube;
    Camera camera;
    Scene<target_t> scene;

public:
    explicit SampleScene(Texture &texture) : colorCube({colorMesh}, FirstShader(), texture),
                                    textureCube({textureMesh}, SecondShader(), texture),
                                    camera(45.0, 0.1, 10, {0, 0, 0}, {0, 0, -3.5}),
                                    scene(camera){
        colorCube.setPosition(-1,0,-3);
        colorCube.setScale(.5,.5,.5);
        textureCube.setPosition(1,0,-3);
        scene.add(colorCube);
        scene.add(textureCube);
    }

    Object<SimpleMesh<SimpleVertex>, SimpleVertex, FirstShader, Texture> &getColorCube()  {
        return colorCube;
    }

    Object<SimpleMesh<TextureVertex>, TextureVertex, SecondShader, Texture> &getTextureCube()  {
        return textureCube;
    }

    const Camera &getCamera() const {
        return camera;
    }

    Scene<target_t> &getScene()  {
        return scene;
    }
};


#endif //INC_3D_RENDERER_SAMPLESCENE_H
