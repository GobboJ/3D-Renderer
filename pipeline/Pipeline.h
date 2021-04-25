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
    std::array<double, 16> projectionMatrix;
public:
    Pipeline(target_t *target, const uint width, const uint height) : target(target), width(width),
                                                                            height(height),
                                                                            projectionMatrix() {}

    void render(const Scene<target_t> &scene) {

        const Camera &camera(scene.getCamera());
        double farPlane = camera.getFarPlane();
        double nearPlane = camera.getNearPlane();
        double aspectRatio = width / (double) height;
        double c = 1.0 / tan(camera.getVerticalFieldOfView() / 2.0);

        projectionMatrix = {c / aspectRatio, 0, 0, 0,
                            0, c, 0, 0,
                            0, 0, -(farPlane + nearPlane) / (farPlane - nearPlane),
                            -(2.0 * farPlane * nearPlane) / (farPlane - nearPlane),
                            0, 0, -1, 0};

        for (const Object &o : scene.getObjects()) {
            o.render(camera.getViewMatrix(), projectionMatrix);
        }
    }

private:
    template<class V, class S>
    void renderTriangle(const V &v1) {//}, const V &v2, const V &v3, S &shader) {

    }
};


#endif //INC_3D_RENDERER_PIPELINE_H