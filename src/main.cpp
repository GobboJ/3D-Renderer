//
// Created by Jonathan on 06/05/2021.
//
#include "pipeline/Scene.h"
#include "pipeline/Camera.h"
#include "pipeline/vertex/SimpleVertex.h"
#include "pipeline/Pipeline.h"
#include <iterator>
#include <array>
#include <cstddef>
#include <iostream>
#include <string>

#define WIDTH 320
#define HEIGHT 240

//Using SDL and standard IO
#include "SDL.h"
#include<windows.h>
#include <chrono>

//Screen dimension constants
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

Uint32 BORDER;
Uint32 OTHER;

class TriangleIterator {
private:
    std::vector<std::array<SimpleVertex, 3>> triangles;
    size_t next;


public:
    // Previously provided by std::iterator - see update below
    typedef std::array<SimpleVertex, 3> value_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type *pointer;
    typedef value_type &reference;
    typedef std::forward_iterator_tag iterator_category;

    TriangleIterator(const std::vector<SimpleVertex> &triangles, const std::vector<std::array<int, 3>> &indices,
                     bool isLast)
            : triangles() {
        for (std::array<int, 3> i : indices) {
            this->triangles.emplace_back(
                    std::array<SimpleVertex, 3>{triangles[i[0]], triangles[i[1]], triangles[i[2]]});
        }
        next = (isLast) ? this->triangles.size() : 0;
    }

    reference operator*() { return triangles.at(next); }

    pointer operator->() { return &(triangles.at(next)); }

    bool operator==(const TriangleIterator &other) const { return next == other.next; }

    bool operator!=(const TriangleIterator &other) const { return next != other.next; }

    // Postfix increment
    TriangleIterator operator++(int) {

        TriangleIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // Prefix increment
    TriangleIterator &operator++() {
        next++;
        return *this;
    }
};

class SimpleMesh {
    std::vector<SimpleVertex> vertices;
    std::vector<std::array<int, 3>> indices;
public:
    SimpleMesh(const std::vector<SimpleVertex> &vertices, const std::vector<std::array<int, 3>> &indices) : vertices(
            vertices), indices(indices) {}

    TriangleIterator begin() const { return TriangleIterator(vertices, indices, false); }

    TriangleIterator end() const { return TriangleIterator(vertices, indices, true); }
};

struct SimpleShader {
    char operator()(SimpleVertex vertex) {

        // double z = fmod(vertex.getZ(), ((int) vertex.getZ()))*10;
        //return '0' + ((int) z);
        return std::to_string(vertex.getZ())[2];
    }
};

struct ColorShader {
    Uint32 operator()(SimpleVertex vertex) {

        // double z = fmod(vertex.getZ(), ((int) vertex.getZ()))*10;
        //return '0' + ((int) z);
        int blue = (int) ((255.0 * (vertex.getZ()))/10);
        //std::cout << blue << " " << vertex.getZ() << std::endl;
        // z : x = 10 : 255
        //return (vertex.isWireframe()) ? BORDER : OTHER;//
        //return (0x00 << 16) | (0x00 << 8) | blue;
        return vertex.getColor();
    }
};

template<class T, class S>
Scene<T> createScene(S &shader, int frame = 0) {
    /*const SimpleMesh rectangle({{0, 0, 0},
                                {4, 0, 0},
                                {0, 2, 0},
                                {4, 2, 0}}, {{0, 1, 2},
                                             {1, 3, 2}});*/

    const SimpleMesh cube({{1,  1,  -1, 0xFF0000},
                           {1,  -1, -1, 0xFFFF00},
                           {1,  1,  1, 0xFF00FF},
                           {1,  -1, 1, 0x0000FF},
                           {-1, 1,  -1, 0x00FFFF},
                           {-1, -1, -1, 0x00FF00},
                           {-1, 1,  1, 0x80FF00},
                           {-1, -1, 1, 0x808080}},
                          {{4, 2, 0},
                           {2, 7, 3},
                           {6, 5, 7},
                          {1, 7, 5},
                          {0, 3, 1},
                          {4, 1, 5},
                          {4, 6, 2},
                          {2, 6, 7},
                          {6, 4, 5},
                          {1, 3, 7},
                          {0, 2, 3},
                          {4, 0, 1}});

    //const Camera camera(45.0, 0.1, 10, {1.5, 1.5, 0}, {1.5, 1.5, -16});
    const Camera camera(45.0, 0.1, 10, {2, 2, 2}, {2, 2, -3.5});

    //SimpleShader shader{};
    Object<SimpleMesh, SimpleVertex, S> o(cube /*rectangle*/, shader);
    o.setPosition(0, 0, -3.5);
//    o.setScale(1, 1, 1);
    o.setRotation(0, frame%360, 0);//
    //std::cout << frame % 360 << std::endl;
    Scene<T> s(camera);
    s.add(o);

    return s;
}

void PrintBoard() {
    // Position cursor at 0,0
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = coord.Y = 0;
    SetConsoleCursorPosition(console, coord);
    // Draw the rest of the stuff.
}

void render_char() {
    /*const SimpleMesh cube({{1,  -1, 1.5},
                           {1,  1,  1.1},
                           {-1, 1,  1.5},
                           {-1, -1, 1.9}}, {{0, 1, 2},
                                            {0, 2, 3}});

    const SimpleMesh rectangle({{0, 0, 0},
                                {4, 0, 0},
                                {0, 2, 0},
                                {4, 2, 0}}, {{0, 1, 2},
                                             {1, 3, 2}});

    const Camera camera(45.0, 0.1, 10, {1.5, 1.5, 0}, {1.5, 1.5, -16});*/
    char target[WIDTH * HEIGHT];
    memset(target, '.', WIDTH * HEIGHT);

    std::chrono::milliseconds last = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    for (int frame = 0; frame < 1000000; frame++) {
        /*SimpleShader shader{};
        Object<SimpleMesh, SimpleVertex, SimpleShader> o(rectangle, shader);
        o.setPosition(0, 0, -6);
        o.setScale(1, 1, 1);
        o.setRotation(0, 0, 0);

        Scene<char> s(camera);
        s.add(o);*/


        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
        );

        SimpleShader shader{};
        Scene<char> s = createScene<char>(shader, frame);
        memset(target, '.', WIDTH * HEIGHT);
        Pipeline<char> p(target, WIDTH, HEIGHT);

        p.render(s);
        /*for (std::array<SimpleVertex, 3> triangle : rectangle) {
            printf("(%.1f %.1f %.1f) (%.1f %.1f %.1f) (%.1f %.1f %.1f)\n", triangle[0].getX(), triangle[0].getY(),
                   triangle[0].getZ(),
                   triangle[1].getX(), triangle[1].getY(), triangle[1].getZ(),
                   triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
        }*/

        // Prints target
        /*PrintBoard();
        for (int r = 0; r < HEIGHT; r++) {
            for (int c = 0; c < WIDTH; c++) {
                std::cout << target[r * WIDTH + c];
            }
            std::cout << '\n';
        }*/
        PrintBoard();
        std::cout << '\n' << (1000.0 / (ms - last).count());
        last = ms;
    }
}

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
            BORDER = SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00);
            OTHER = SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF);
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
                        colorTarget[y * WIDTH + x] = (0xFF << 16) | (0xFF << 8) | 0xFF;
                    }
                }
                //if (frame == 1) {
                    render_color(screenSurface, frame, colorTarget);
                    //quit = true;
                //}
                unsigned int currentTime = SDL_GetTicks();
                // 10ms : 1 = 1000ms : x

                SDL_SetWindowTitle(window, (std::to_string(1000.0 / (currentTime - lastTime)) + " fps").c_str());

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
    //render_char();

    return 0;
}
