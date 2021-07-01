//
// Created by Jonathan on 10/05/2021.
//

#ifndef INC_3D_RENDERER_TEXTURESHADER_H
#define INC_3D_RENDERER_TEXTURESHADER_H

#include "../pipeline/vertex/TextureVertex.h"

// TODO Move all to Shaders.h
struct myTexture {
    unsigned int w{};
    unsigned int h{};
    std::vector<Uint32> texture;
};

struct TextureShader {

    Uint32 operator()(TextureVertex &vertex, std::tuple<myTexture> &textures) {

        myTexture &texture = std::get<0>(textures);
        unsigned int textureWidth = texture.w;
        unsigned int textureHeight = texture.h;

        unsigned int mappedX = (int) (vertex.getU() * textureWidth);
        unsigned int mappedY = (int) (vertex.getV() * textureHeight);

        return texture.texture[std::min(mappedY * textureWidth + mappedX, (unsigned int) (texture.texture.size()-1))];
    }
};


#endif //INC_3D_RENDERER_TEXTURESHADER_H
