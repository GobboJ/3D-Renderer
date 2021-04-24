//
// Created by Jonathan on 09/04/2021.
//

#ifndef INC_3D_RENDERER_SCENE_H
#define INC_3D_RENDERER_SCENE_H

#include <utility>
#include <vector>
#include "object/Object.h"
#include "Camera.h"

template<class target_t>
class Scene {

private:
    std::vector<Object> objects;
    Camera camera;
public:
    const std::vector<Object> &getObjects() const {
        return objects;
    }

    Scene(std::vector<Object> &objects, const Camera &camera) : objects(
            std::move(objects)), camera(camera) {}

    std::array<double, 16> getViewMatrix() const {
        return camera.getViewMatrix();
    }

};


#endif //INC_3D_RENDERER_SCENE_H
