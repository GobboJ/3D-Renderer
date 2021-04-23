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
    std::vector<std::tuple<Object, Vector3, Vector3>> objects;
    Camera camera;
public:
    const std::vector<std::tuple<Object, Vector3, Vector3>> &getObjects() const {
        return objects;
    }

public:
    Scene(std::vector<std::tuple<Object, Vector3, Vector3>> objects, const Camera &camera) : objects(
            std::move(objects)), camera(camera) {}
};


#endif //INC_3D_RENDERER_SCENE_H
