//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_PIPELINE_H
#define INC_3D_RENDERER_PIPELINE_H

#include "Scene.h"

/**
 * Represents the rendering pipeline
 * @tparam target_t The type of the target
 */
template<class target_t>
class Pipeline {

private:

    /**
     * Target
     */
    target_t *target;

    /**
     * Width of the target
     */
    const unsigned int width;

    /**
     * Height of the target
     */
    const unsigned int height;

    /**
     * Z-Buffer of the target
     */
    double *z_buffer;

public:

    /**
     * Constructor of Pipeline
     * @param target The target to render to
     * @param width The width of the target
     * @param height The height of the target
     */
    Pipeline(target_t *target, const unsigned int width, const unsigned int height) : target(target), width(width),
                                                                                      height(height) {
        // Allocates the z-buffer
        z_buffer = new double[width * height];
        // And initializes every "pixel" to 1.0
        std::fill_n(z_buffer, width * height, 1.0);
    }

    /**
     * Deconstructor of Pipeline
     */
    ~Pipeline() {
        // Deallocates the z-buffer
        delete[] z_buffer;
    }

    /**
     * Renders the scene
     * @param scene Scene to be rendered
     */
    int render(Scene<target_t> &scene) {
        // Renders the scene
        int rendered_objects = scene.render(target, z_buffer, width, height);
        // Cleans the z-buffer
        std::fill_n(z_buffer, width * height, 1.0);
        return rendered_objects;
    }

};


#endif //INC_3D_RENDERER_PIPELINE_H
