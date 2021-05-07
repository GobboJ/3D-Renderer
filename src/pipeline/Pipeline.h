//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_PIPELINE_H
#define INC_3D_RENDERER_PIPELINE_H

#include "Scene.h"

template <class target_t>
class Pipeline {
private:
    target_t *target;
    const unsigned int width;
    const unsigned int height;

    double *z_buffer;

public:
    Pipeline(target_t *target, const unsigned int width, const unsigned int height) : target(target), width(width),
                                                                                      height(height) {
        z_buffer = new double[width * height];
        std::fill_n(z_buffer, width * height, 1.0);
    }

    ~Pipeline() {
        delete[] z_buffer;
    }

    void render(Scene<target_t> &scene) {


        scene.render(target, z_buffer, width, height);

        memset(z_buffer, 0, width * height);
    }
};


#endif //INC_3D_RENDERER_PIPELINE_H
