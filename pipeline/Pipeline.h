//
// Created by Jonathan on 16/04/2021.
//

#ifndef INC_3D_RENDERER_PIPELINE_H
#define INC_3D_RENDERER_PIPELINE_H

#include "Scene.h"

typedef unsigned int uint;

template<class target_t>
class Pipeline {
private:
    target_t *target;
    double *z_buffer;
    uint width;
    uint height;

public:
    Pipeline(target_t *target, uint width, uint height);

    void render(const Scene<target_t> &scene);
};

template<class target_t>
Pipeline<target_t>::Pipeline(target_t *target, uint width, uint height) : target(target), width(width),
                                                                          height(height) {}

template<class target_t>
void Pipeline<target_t>::render(const Scene<target_t> &scene) {
    for (const Object &o : scene.getObjects()) {
        o.render(scene.getViewMatrix());
    }
}

template<class V, class S>
void renderTriangle(const V &v1) {//}, const V &v2, const V &v3, S &shader) {

}


#endif //INC_3D_RENDERER_PIPELINE_H