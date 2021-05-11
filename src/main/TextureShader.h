//
// Created by Jonathan on 10/05/2021.
//

#ifndef INC_3D_RENDERER_TEXTURESHADER_H
#define INC_3D_RENDERER_TEXTURESHADER_H

#include "SDL.h"
#include "../pipeline/vertex/TextureVertex.h"

Uint32 getpixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *) p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *) p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}


struct TextureShader {

    Uint32 operator()(TextureVertex vertex, std::tuple<SDL_Surface *> textures) {

        SDL_Surface *texture = std::get<0>(textures);
        int textureWidth = texture->w;
        int textureHeight = texture->h;

        return getpixel(texture, (int) (vertex.getU() * textureWidth), (int) (vertex.getV() * textureHeight));
    }
};


#endif //INC_3D_RENDERER_TEXTURESHADER_H
