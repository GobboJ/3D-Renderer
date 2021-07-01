//
// Created by Jonathan on 06/05/2021.
//

#include "../pipeline/Scene.h"
#include "../pipeline/Camera.h"
#include "../pipeline/Pipeline.h"
#include "SDL.h"
#include "SceneBuilder.h"
#include "SampleScene.h"
#include <iterator>
#include <cstddef>
#include <iostream>
#include <string>

/**
 * Window size
 */
#define WIDTH 640
#define HEIGHT 480


// TODO Split in two, shader and scene before main loop, render inside
void render_color(SDL_Surface *screenSurface, myTexture &texture, int frame, Uint32 *colorTarget) {

    // Creates the texture shader
    TextureShader shader{};

    // Creates a scene
    Scene<Uint32> s = createTextureScene<Uint32, TextureShader, myTexture>(shader, texture, frame);

    // Creates the pipeline and renders the scene
    Pipeline<Uint32> p(colorTarget, WIDTH, HEIGHT);
    p.render(s);

    // Copies the output target on the surface
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


/**
 * Returns the color of the surface at coordinate (x,y)
 */
Uint32 getpixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *) p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *) p;
            break;
        default:
            return 0;
    }
}


/**
 * Render a scene on the SDL window
 */
void render_window() {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {

        // Creates the window
        SDL_Window *window = SDL_CreateWindow("3D-Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH,
                                              HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        } else {

            // Quit application flag
            bool quit = false;
            // Event handler
            SDL_Event e;

            // Frame counting
            unsigned int frame = 0;
            unsigned int lastTime = 0;
            unsigned int totalTime = 0;
            unsigned int totalFrames = 0;
            unsigned int currentTime;
            unsigned int difference;

            double velocity = 0.1;

            Uint32 *colorTarget = new Uint32[WIDTH * HEIGHT];
            // Get the surface contained by the window
            SDL_Surface *screenSurface = SDL_GetWindowSurface(window);


            // Reads the texture
            // TODO Move texture reading somewhere else
            SDL_Surface *texture = SDL_LoadBMP("texture.bmp");
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

            SampleScene<Uint32, ColorShader, TextureShader, myTexture> sampleScene(t);
            Pipeline<Uint32> p(colorTarget, WIDTH, HEIGHT);


            // Main loop
            while (!quit) {

                start_chrono(1);

                // Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    // Quit event received
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                // Fill the surface white
                SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
                for (int y = 0; y < HEIGHT; y++) {
                    for (int x = 0; x < WIDTH; x++) {
                        colorTarget[y * WIDTH + x] = 0xFFFFFF;
                    }
                }

                // Computations to fix timesteps
                currentTime = SDL_GetTicks();
                difference = currentTime - lastTime;
                frame += difference;

                //
                //render_color(screenSurface, t, velocity * frame, colorTarget);
                sampleScene.getColorCube().setRotation(velocity * frame, 0 ,0);
                sampleScene.getTextureCube().setRotation(0, velocity * frame, 0);

                p.render(sampleScene.getScene());
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

                // Computations to find fps
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
