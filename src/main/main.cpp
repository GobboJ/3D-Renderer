//
// Created by Jonathan on 06/05/2021.
//

#include "../pipeline/Scene.h"
#include "../pipeline/Camera.h"
#include "../pipeline/vertex/SimpleVertex.h"
#include "../pipeline/Pipeline.h"
#include "Shaders.h"
#include "SceneBuilder.h"
#include "SampleScene.h"
#include <iostream>
#include <cstring>

/**
 * Window size
 */
#define WIDTH 150
#define HEIGHT 50


/**
 * Render a scene on the Terminal
 */
void render_char() {

    // Creates the target and set every "pixel" to '.'
    char target[WIDTH * HEIGHT];
    memset(target, '.', WIDTH * HEIGHT);

    // Creates the char shader and a simple scene
    //CharShader shader{};
    //Scene<char> s = createSimpleScene<char, CharShader, char>(shader, 'a');
    myTexture t;
    SampleScene<char, CharShader, CharShader, myTexture> sampleScene(t);


    // Creates the pipeline and renders the scene
    Pipeline<char> p(target, WIDTH, HEIGHT);
    p.render(sampleScene.getScene());


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
