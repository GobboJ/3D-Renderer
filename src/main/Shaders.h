//
// Created by Jonathan on 09/05/2021.
//

#ifndef INC_3D_RENDERER_SHADERS_H
#define INC_3D_RENDERER_SHADERS_H

#include "../pipeline/Scene.h"
#include "../pipeline/Camera.h"
#include "../pipeline/vertex/SimpleVertex.h"
#include "../pipeline/Pipeline.h"
#include "../pipeline/vertex/TextureVertex.h"
#include <iterator>
#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <cmath>

/**
 * Simple texture type
 */
struct myTexture {

    // Width of the image
    unsigned int w{};

    // Height of the image
    unsigned int h{};

    // Image data
    std::vector<uint32_t> texture;
};

/**
 * Shader to be applied on a SimpleVertex or TextureVertex, outputs to a char target
 */
struct CharShader {

    /**
     * Applies the shader on a SimpleVertex
     * @param vertex The SimpleVertex the shader is applied to
     * @param texture Unused but necessary parameter
     * @return The char to be written on the target
     */
    char operator()(SimpleVertex vertex, std::tuple<myTexture> &texture) {

        return static_cast<char>((abs(vertex.getZ())) * 10.0 + 0.5) % 10 + '0';

    }

    /**
     * Applies the shader on a TextureVertex
     * @param vertex The TextureVertex the shader is applied to
     * @param texture Unused but necessary parameter
     * @return The char to be written on the target
     */
    char operator()(TextureVertex vertex, std::tuple<myTexture> &texture) {

        return static_cast<char>((abs(vertex.getZ())) * 10.0 + 0.5) % 10 + '0';

    }
};


/**
 * Shader to be applied to a SimpleVertex, outputs to a color (uint32_t) target
 */
struct ColorShader {

    /**
     * Applies the shader on a SimpleVertex
     * @param vertex The SimpleVertex the shader is applied to
     * @param texture Unused but necessary parameter
     * @return The uint32_t to be written on the target
     */
    uint32_t operator()(SimpleVertex vertex, std::tuple<myTexture> &texture) {

        return vertex.getColor();

    }
};

/**
 * Shader to be applied to a TextureVertex, outputs to a color (uint32_t) target
 */
struct TextureShader {

    /**
     * Applies the shader on a TextureVertex
     * @param vertex The TextureVertex the shader is applied to
     * @param textures The textures to be applied on the vertex
     * @return The uint32_t to be written on the target
     */
    uint32_t operator()(TextureVertex &vertex, std::tuple<myTexture> &textures) {

        // Extracts the first texture and its dimensions
        myTexture &texture = std::get<0>(textures);
        unsigned int textureWidth = texture.w;
        unsigned int textureHeight = texture.h;

        // Computes the position of the pixel to be extracted
        unsigned int mappedX = (int) (vertex.getU() * textureWidth);
        unsigned int mappedY = (int) (vertex.getV() * textureHeight);

        // Extracts the pixel from the texture
        return texture.texture[std::min(mappedY * textureWidth + mappedX, (unsigned int) (texture.texture.size() - 1))];
    }
};

#endif //INC_3D_RENDERER_SHADERS_H
