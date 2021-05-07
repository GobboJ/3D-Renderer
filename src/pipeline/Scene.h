//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_SCENE_H
#define INC_3D_RENDERER_SCENE_H

#include <vector>
#include <memory>
#include "Camera.h"
#include "DedicatedPipeline.h"
#include "DedicatedPipelineImpl.h"
#include "object/Object.h"

template<class target_t>
class Scene {
private:
    Camera camera;
    std::vector<std::unique_ptr<DedicatedPipeline<target_t>>> pipelines;

public:
    Scene(const Camera &camera) : camera(camera), pipelines() {}

    template<class Mesh, class Vertex, class Shader, class ...Texture>
    void add(const Object<Mesh, Vertex, Shader, Texture...> &o) {
        pipelines.push_back(
                std::make_unique<DedicatedPipelineImpl<target_t, Mesh, Vertex, Shader, Texture...>>(o.getMesh(),
                                                                                                    o.getShader(),
                                                                                                    o.getTextures(),
                                                                                                    o.getInfo()));
    }

    void render(target_t *target, double *z_buffer, const unsigned int width, const unsigned int height) {

        double farPlane = camera.getFarPlane();
        double nearPlane = camera.getNearPlane();
        double aspectRatio = width / (double) height;
        double c = 1.0 / tan(camera.getVerticalFieldOfView() / 2.0);

        std::array<double, 16> projectionMatrix = {c / aspectRatio, 0, 0, 0,
                                                   0, c, 0, 0,
                                                   0, 0, -(farPlane + nearPlane) / (farPlane - nearPlane),
                                                   -(2.0 * farPlane * nearPlane) / (farPlane - nearPlane),
                                                   0, 0, -1, 0};

        // Supposes lower-left corner of the viewport as (0,0)
        std::array<double, 16> viewportMatrix = {width / 2.0, 0, 0, width / 2.0,
                                                 0, height / 2.0, 0, height / 2.0,
                                                 0, 0, (farPlane - nearPlane) / 2, (nearPlane + farPlane) / 2,
                                                 0, 0, 0, 1};

        for (auto &pipeline : pipelines) {
            pipeline->render(target, z_buffer, width, height, camera.getViewMatrix(), projectionMatrix, viewportMatrix);
        }
    }

    // TODO: memory management
    // TODO: To change Camera or not to change?
};

#endif //INC_3D_RENDERER_SCENE_H
