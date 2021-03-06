//
// Created by Jonathan on 06/05/2021.
//

#include "../pipeline/Scene.h"
#include "../pipeline/Camera.h"
#include "../pipeline/Pipeline.h"
#include "SDL.h"
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

            // Speed of the objects
            double velocity = 0.1;

            // Target
            Uint32 *colorTarget = new Uint32[WIDTH * HEIGHT];

            // Get the surface contained by the window
            SDL_Surface *screenSurface = SDL_GetWindowSurface(window);


            // Reads the texture
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

            // Creates the example scene and the pipeline
            SampleScene<Uint32, ColorShader, TextureShader, myTexture> sampleScene(t);
            Pipeline<Uint32> p(colorTarget, WIDTH, HEIGHT);

            // Flag for animation
            bool set = false;

            // Main loop
            while (!quit) {

                // start_chrono(1);

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

                // Transforms the two cubes each frame
                sampleScene.getColorCube().setRotation(((int) (velocity * frame)) % 360, 0, 0);
                sampleScene.getTextureCube().setRotation(0, ((int) (velocity * frame)) % 360, 0);
                sampleScene.getAnotherColorCube().setPosition((velocity * frame / 100) - 5, 0, -8);
                sampleScene.getAnotherColorCube().setRotation(((int) (velocity * frame)) % 360,
                                                              ((int) (velocity * frame)) % 360,
                                                              ((int) (velocity * frame)) % 360);

                // When the moving object goes out of scene, transforms the color cube
                if (!set && velocity * frame > 1350) {
                    sampleScene.getColorCube().nextAnimationFrame();
                    set = true;
                }

                // Renders the scene and copies the target on the window surface
                int rendered_objects = p.render(sampleScene.getScene());
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
                SDL_SetWindowTitle(window,
                                   ("3D-Renderer [ Visible objects: " + std::to_string(rendered_objects) + " - " +
                                    std::to_string(1000.0 / difference) +
                                    " fps ]").c_str());

                lastTime = currentTime;
                // stop_chrono(1);
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

/**
 * Prints performance info
 */
void print_chrono_info(const char *desc, int index) {

    std::cout << desc << " " << getAverageMilliseconds(index) << " ms (" << getAverageMicroseconds(index)
              << " micros --> " << getAverageNanoseconds(index) << " nanos)" << std::endl;
}

/**
 * Main
 */
int main(int argc, char *argv[]) {
    render_window();

    // Prints performance info of each operation
    /*
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
    */
    return 0;
}
