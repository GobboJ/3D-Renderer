//
// Created by Jonathan on 06/05/2021.
//
#include "../pipeline/Scene.h"
#include "../pipeline/Camera.h"
#include "../pipeline/vertex/SimpleVertex.h"
#include "../pipeline/Pipeline.h"
#include "SDL.h"
#include "Shaders.h"
#include "SceneBuilder.h"
#include <iterator>
#include <cstddef>
#include <iostream>
#include <string>

#define WIDTH 640
#define HEIGHT 480

//Screen dimension constants
const int SCREEN_WIDTH = WIDTH;
const int SCREEN_HEIGHT = HEIGHT;



void render_color(SDL_Surface *screenSurface, int frame, Uint32 *colorTarget) {

    ColorShader shader{};
    Scene<Uint32> s = createScene<Uint32>(shader, frame);
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
            Uint32 *colorTarget = new Uint32[WIDTH * HEIGHT];
            //Get window surface The surface contained by the window
            SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
            //While application is running
            while (!quit) {
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
                render_color(screenSurface, frame, colorTarget);

                unsigned int currentTime = SDL_GetTicks();

                SDL_SetWindowTitle(window, ("3D-Renderer [ " + std::to_string(1000.0 / (currentTime - lastTime)) +
                                            " fps ]").c_str());

                lastTime = currentTime;
                frame++;
                // Update the surface
                SDL_UpdateWindowSurface(window);
            }


        }
        // Destroy window
        SDL_DestroyWindow(window);
        // Quit SDL subsystems
        SDL_Quit();
    }
}

int main(int argc, char *argv[]) {

    render_window();

    return 0;
}
