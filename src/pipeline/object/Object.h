//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_OBJECT_H
#define INC_3D_RENDERER_OBJECT_H

#include <tuple>
#include "ObjectInfo.h"

template<class Mesh, class Vertex, class Shader, class ...Texture>
class Object {

private:
    const Mesh mesh;
    const Shader shader;
    const std::tuple<Texture...> textures;
    ObjectInfo *info;
    //TODO: Memory management: shared_ptr

public:
    Object(const Mesh &mesh, const Shader &shader, const Texture& ...textures) : mesh(mesh),
                                                                               shader(shader),
                                                                               textures(textures...),
                                                                               info(new ObjectInfo()) {}

    void setPosition(const double x, const double y, const double z) {
        info->setPosition(x, y, z);
    }

    void setRotation(const double rx, const double ry, const double rz) {
        info->setRotation(rx, ry, rz);
    }

    void setScale(const double sx, const double sy, const double sz) {
        info->setScale(sx, sy, sz);
    }

    Mesh getMesh() const {
        return mesh;
    }

    Shader getShader() const {
        return shader;
    }

    const std::tuple<Texture...> &getTextures() const {
        return textures;
    }

    ObjectInfo *getInfo() const {
        return info;
    }
/**
 * void createObj(Scene &scene, SimpleMesh m, ...) {
 *      Object<...> o = new Object(m, ....);
 *
 *      //scene.add(o);
 *      o.setX(10);
 *      //delete o;
 * }
 */
};

#endif //INC_3D_RENDERER_OBJECT_H
