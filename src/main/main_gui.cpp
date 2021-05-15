//
// Created by Jonathan on 06/05/2021.
//
#include "../pipeline/Scene.h"
#include "../pipeline/Camera.h"
#include "../pipeline/Pipeline.h"
#include "SDL.h"
#include "SceneBuilder.h"
#include "TextureShader.h"
#include <iterator>
#include <cstddef>
#include <iostream>
#include <string>

#define WIDTH 640
#define HEIGHT 480

//Screen dimension constants
const int SCREEN_WIDTH = WIDTH;
const int SCREEN_HEIGHT = HEIGHT;




void render_color(SDL_Surface *screenSurface, myTexture &texture, int frame, Uint32 *colorTarget) {

    TextureShader shader{};

    Scene<Uint32> s = createTextureScene<Uint32, TextureShader, myTexture>(shader, texture, frame);
    Pipeline<Uint32> p(colorTarget, WIDTH, HEIGHT);
    p.render(s);

    SDL_Rect pixel;
    pixel.w = 1;
    pixel.h = 1;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            pixel.x = x;
            pixel.y = y;
            if (SDL_FillRect(screenSurface, &pixel, colorTarget[y * WIDTH + x])) {
                std::cerr << "Error!";
            }
        }
    }
}

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

void render_window() {
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {

        // Create the window The window we'll be rendering to
        SDL_Window *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                              SCREEN_WIDTH,
                                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;
            int frame = 0;
            unsigned int lastTime = 0;
            unsigned int totalTime = 0;
            unsigned int totalFrames = 0;
            Uint32 *colorTarget = new Uint32[WIDTH * HEIGHT];
            //Get window surface The surface contained by the window
            SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
            SDL_Surface *texture = SDL_LoadBMP("texture3.bmp");
            if (texture == nullptr) {
                std::cerr << "Missing textures!!";
                exit(1);
            }
            myTexture t;
            t.h = texture->h;
            t.w = texture->w;
            for (int y = 0; y < texture->h; y++) {
                for (int x = 0; x < texture->w; x++) {
                    t.texture.push_back(getpixel(texture, x, y));
                }
            }
            //While application is running
            while (!quit) {
                start_chrono(1);
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                //Fill the surface white
                SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
                for (int y = 0; y < HEIGHT; y++) {
                    for (int x = 0; x < WIDTH; x++) {
                        colorTarget[y * WIDTH + x] = 0xFFFFFF;
                    }
                }
                render_color(screenSurface, t, frame, colorTarget);


                unsigned int currentTime = SDL_GetTicks();

                unsigned int difference = currentTime - lastTime;
                totalTime += difference;
                totalFrames++;
                SDL_SetWindowTitle(window, ("3D-Renderer [ " + std::to_string(1000.0 / difference) +
                                            " fps ]").c_str());

                lastTime = currentTime;
                stop_chrono(1);
                frame++;
                // Update the surface
                SDL_UpdateWindowSurface(window);
            }

            std::cout << "AVG FRAMES: " << (((totalFrames / (double) totalTime)) * 1000) << "fps" << std::endl;

        }
        // Destroy window
        SDL_DestroyWindow(window);
        // Quit SDL subsystems
        SDL_Quit();
    }
}

void print_chrono_info(const char *desc, int index) {

    std::cout << desc << " " << getAverageMilliseconds(index) << " ms (" << getAverageMicroseconds(index)
              << " micros --> " << getAverageNanoseconds(index) << " nanos)" << std::endl;
}

int main(int argc, char *argv[]) {
    render_window();

    print_chrono_info("Render:      ", 0);
    print_chrono_info("Render2:     ", 1);
    print_chrono_info("Baricentric  ", 2);
    print_chrono_info("Transform    ", 3);
    print_chrono_info("Interpolation", 4);
    print_chrono_info("Shading      ", 5);
    print_chrono_info("Z-Buffer     ", 6);
    print_chrono_info("Triangle     ", 7);
    print_chrono_info("Row          ", 8);
    print_chrono_info("Baricentric 1", 9);
    //print_chrono_info("Baricentric 2", 10);
    return 0;
}
