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

/**
 * Represents a scene
 * @tparam target_t Type of the target
 */
template<class target_t>
class Scene {

private:

    // Camera in the scene
    Camera camera;
    // Dedicated pipelines for each object
    std::vector<std::unique_ptr<DedicatedPipeline<target_t>>> pipelines;

public:

    /**
     * Constructor of Scene
     * @param camera The camera in the scene
     */
    explicit Scene(const Camera &camera) : camera(camera), pipelines() {}

    /**
     * Add an object to the scene
     * @tparam Mesh The type of the mesh
     * @tparam Vertex The type of the vertices of the object
     * @tparam Shader The type of the shader to be applied to the object
     * @tparam Texture The types of the textures to be applied to the object
     * @param o The object to be added in the scene
     */
    template<class Mesh, class Vertex, class Shader, class ...Texture>
    void add(const Object<Mesh, Vertex, Shader, Texture...> &o) {
        // Creates the dedicated pipeline for the new object and adds it to the vector
        pipelines.push_back(
                std::make_unique<DedicatedPipelineImpl<target_t, Mesh, Vertex, Shader, Texture...>>(o.getMeshes(),
                                                                                                    o.getShader(),
                                                                                                    o.getTextures(),
                                                                                                    o.getBoundingSphere(),
                                                                                                    o.getInfo()));
    }

    /**
     * Renders the scene
     * @param target The target on which the scene is rendered
     * @param z_buffer The z-buffer of the target
     * @param width The width of the target
     * @param height The height of the target
     */
    int render(target_t *target, double *z_buffer, const unsigned int width, const unsigned int height) {

        // Gets camera information
        double farPlane = camera.getFarPlane();
        double nearPlane = camera.getNearPlane();
        double aspectRatio = width / (double) height;
        double c = 1.0 / tan(camera.getVerticalFieldOfView() / 2.0);

        //  Computes the projection matrix
        std::array<double, 16> projectionMatrix = {c / aspectRatio, 0, 0, 0,
                                                   0, c, 0, 0,
                                                   0, 0, -((farPlane + nearPlane) / (farPlane - nearPlane)),
                                                   -((2.0 * farPlane * nearPlane) / (farPlane - nearPlane)),
                                                   0, 0, -1, 0};

        // Computes  the viewport matrix
        // Supposes lower-left corner of the viewport as (0,0)
        std::array<double, 16> viewportMatrix = {width / 2.0, 0, 0, width / 2.0,
                                                 0, height / 2.0, 0, height / 2.0,
                                                 0, 0, -(farPlane - nearPlane) / 2, -(nearPlane + farPlane) / 2,
                                                 0, 0, 0, 1};

        // Calls the appropriate render method for each dedicated pipeline
        int rendered_objects = 0;
        for (auto &pipeline : pipelines) {
            bool rendered = pipeline->render(target, z_buffer, width, height, camera.getViewMatrix(), projectionMatrix,
                                             viewportMatrix);
            rendered_objects += (rendered) ? 1 : 0;
        }
        return rendered_objects;
    }

    // TODO: memory management
    // TODO: update docs
};

#endif //INC_3D_RENDERER_SCENE_H
