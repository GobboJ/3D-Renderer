//
// Created by Jonathan on 06/05/2021.
//

#include "../pipeline/Scene.h"
#include "../pipeline/Camera.h"
#include "../pipeline/vertex/SimpleVertex.h"
#include "../pipeline/Pipeline.h"
#include "Shaders.h"
#include "SceneBuilder.h"
#include <iterator>
#include <iostream>

#define WIDTH 150
#define HEIGHT 50


void render_char() {

    char target[WIDTH * HEIGHT];
    memset(target, '.', WIDTH * HEIGHT);


    CharShader shader{};
    Scene<char> s = createSimpleScene<char, CharShader, char>(shader, 'a');
    Pipeline<char> p(target, WIDTH, HEIGHT);

    p.render(s);


    // Prints target
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            std::cout << target[r * WIDTH + c];
        }
        std::cout << '\n';
    }
}

int main(int argc, char *argv[]) {
    render_char();
    return 0;
}
