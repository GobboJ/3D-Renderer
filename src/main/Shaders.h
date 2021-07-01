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

struct myTexture {
    unsigned int w{};
    unsigned int h{};
    std::vector<uint32_t> texture;
};

struct CharShader {
    char operator()(SimpleVertex vertex, std::tuple<myTexture> &texture) {

        return static_cast<char>((abs(vertex.getZ())) * 10.0 + 0.5) % 10 + '0';
        //return std::to_string(vertex.getZ())[3];
    }
    char operator()(TextureVertex vertex, std::tuple<myTexture> &texture) {

        return static_cast<char>((abs(vertex.getZ())) * 10.0 + 0.5) % 10 + '0';
        //return std::to_string(vertex.getZ())[3];
    }
};


struct ColorShader {
    uint32_t operator()(SimpleVertex vertex, std::tuple<myTexture> &texture) {

        return vertex.getColor();
    }
};




struct TextureShader {

    uint32_t operator()(TextureVertex &vertex, std::tuple<myTexture> &textures) {

        myTexture &texture = std::get<0>(textures);
        unsigned int textureWidth = texture.w;
        unsigned int textureHeight = texture.h;

        unsigned int mappedX = (int) (vertex.getU() * textureWidth);
        unsigned int mappedY = (int) (vertex.getV() * textureHeight);

        return texture.texture[std::min(mappedY * textureWidth + mappedX, (unsigned int) (texture.texture.size()-1))];
    }
};

#endif //INC_3D_RENDERER_SHADERS_H
