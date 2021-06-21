//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_OBJECT_H
#define INC_3D_RENDERER_OBJECT_H

#include <tuple>
#include <vector>
#include "ObjectInfo.h"
#include "../util/bounding_sphere.h"


template<class Mesh, class Vertex, class Shader, class ...Texture>
class Object {

private:
    const std::vector<Mesh> mesh;
    const Shader shader;
    const std::tuple<Texture...> textures;
    ObjectInfo *info;
    //TODO: Memory management: shared_ptr

    std::vector<bounding_sphere> boundingSpheres;

public:
    Object(const std::vector<Mesh> &mesh, const Shader &shader, const Texture &...textures) : mesh(mesh),
                                                                                              shader(shader),
                                                                                              textures(textures...),
                                                                                              info(new ObjectInfo(
                                                                                                      mesh.size())),
                                                                                              boundingSpheres() {
        // TODO For each animation frame, compute bounding sphere
        for (auto &m : mesh) {
            std::vector<Vertex> vertices = m.getVertices();
            double maxDistance = 0, distance;
            int firstIndex = 0, secondIndex = 0;
            for (int i = 0; i < vertices.size(); i++) {
                for (int j = 0; j < vertices.size(); j++) {
                    distance = vertices[i].distance(vertices[j]);
                    if (distance > maxDistance) {
                        maxDistance = distance;
                        firstIndex = i;
                        secondIndex = j;
                    }
                }
            }
            bounding_sphere b{maxDistance / 2, (vertices[firstIndex].getX() + vertices[secondIndex].getX()) / 2,
                              (vertices[firstIndex].getY() + vertices[secondIndex].getY()) / 2,
                              (vertices[firstIndex].getZ() + vertices[secondIndex].getZ()) / 2};
            boundingSpheres.emplace_back(b);
        }
    }

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
        return mesh[info->getCurrentAnimationStep()];
    }

    bounding_sphere getBoundingSphere() const {
        return boundingSpheres[info->getCurrentAnimationStep()];
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

    void nextAnimationFrame() {
        info->nextAnimationFrame();
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
