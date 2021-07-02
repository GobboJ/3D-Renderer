//
// Created by Jonathan on 01/07/2021.
//

#ifndef INC_3D_RENDERER_SAMPLESCENE_H
#define INC_3D_RENDERER_SAMPLESCENE_H


#include "../pipeline/object/Object.h"
#include "SimpleMesh.h"
#include "Shaders.h"

/**
 * Generates an example scene with arbitrary shaders and textures on an arbitrary target
 * @tparam target_t The type of the target
 * @tparam FirstShader The type of the first shader
 * @tparam SecondShader The type of the second shader
 * @tparam Texture The type of the texture
 */
template<class target_t, class FirstShader, class SecondShader, class ...Texture>
class SampleScene {

private:

    /**
     * Mesh for a colored cube without textures
     */
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

    /**
     * Mesh for a texture cube.
     * Uses replicated vertices in order to apply the texture correctly
     */
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

    /**
     * Colored cube
     */
    Object<SimpleMesh<SimpleVertex>, SimpleVertex, FirstShader, Texture...> colorCube;

    /**
     * Textured cube
     */
    Object<SimpleMesh<TextureVertex>, TextureVertex, SecondShader, Texture...> textureCube;

    /**
     * Camera of the scene
     */
    Camera camera;

    /**
     * Generated scene
     */
    Scene<target_t> scene;

public:

    /**
     * Constructor of SampleScene
     * @param texture Texture to be applied on the textured cube
     */
    explicit SampleScene(Texture& ...texture) : colorCube({colorMesh}, FirstShader(), texture...),
                                                textureCube({textureMesh}, SecondShader(), texture...),
                                                camera(45.0, 0.1, 10, {0, 0, 0}, {0, 0, -3.5}),
                                                scene(camera) {

        // Sets position and scale of the cubes
        colorCube.setPosition(-1, 0, -3);
        colorCube.setScale(.5, .5, .5);
        textureCube.setPosition(1, 0, -3);

        // Adds the cubes to the scene
        scene.add(colorCube);
        scene.add(textureCube);
    }

    /**
     * Returns the colored cube object
     * @return the colored cube object
     */
    Object<SimpleMesh<SimpleVertex>, SimpleVertex, FirstShader, Texture...> &getColorCube() {
        return colorCube;
    }

    /**
     * Returns the textured cube object
     * @return the textured cube object
     */
    Object<SimpleMesh<TextureVertex>, TextureVertex, SecondShader, Texture...> &getTextureCube() {
        return textureCube;
    }

    /**
     * Returns the camera of the scene
     * @return the camera of the scene
     */
    const Camera &getCamera() const {
        return camera;
    }

    /**
     * Returns the generated scene
     * @return the generated scene
     */
    Scene<target_t> &getScene() {
        return scene;
    }
};


#endif //INC_3D_RENDERER_SAMPLESCENE_H
