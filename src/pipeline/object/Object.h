//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_OBJECT_H
#define INC_3D_RENDERER_OBJECT_H

#include <tuple>
#include <vector>
#include "ObjectInfo.h"
#include "../util/BoundingSphere.h"

/**
 * Describes an Object of the scene
 * @tparam Mesh Type of the meshes
 * @tparam Vertex Type of the vertices
 * @tparam Shader Type of the shader
 * @tparam Texture Types of the textures
 */
template<class Mesh, class Vertex, class Shader, class ...Texture>
class Object {

private:

    /**
     * Vector of meshes (animation frames)
     */
    const std::vector<Mesh> mesh;

    /**
     * Shader to be applied to the object
     */
    const Shader shader;


    /**
     * Textures to be applied to the object
     */
    const std::tuple<Texture...> textures;

    /**
     * Bounding spheres for each mesh
     */
    std::vector<bounding_sphere> boundingSpheres;

    /**
     * Metadata of the object
     */
    ObjectInfo *info;


public:

    /**
     * Object Constructor
     * @param mesh Vector of meshes
     * @param shader Shader to be applied to the object
     * @param textures Textures to be applied to the object
     */
    Object(const std::vector<Mesh> &mesh, const Shader &shader, const Texture &...textures) : mesh(mesh),
                                                                                              shader(shader),
                                                                                              textures(textures...),
                                                                                              info(new ObjectInfo(
                                                                                                      mesh.size())),
                                                                                              boundingSpheres() {
        // For each mesh, compute its bounding sphere
        for (auto &m : mesh) {
            std::vector<Vertex> vertices = m.getVertices();
            double maxDistance = 0, distance;
            int firstIndex = 0, secondIndex = 0;
            // Finds maximum distance between each pair of vertices of the mesh
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
            // Computes the center and radius of the bounding sphere
            bounding_sphere b{maxDistance / 2, (vertices[firstIndex].getX() + vertices[secondIndex].getX()) / 2,
                              (vertices[firstIndex].getY() + vertices[secondIndex].getY()) / 2,
                              (vertices[firstIndex].getZ() + vertices[secondIndex].getZ()) / 2};
            boundingSpheres.emplace_back(b);
        }
    }

    /**
     * Sets position of the object to (x,y,z)
     */
    void setPosition(const double x, const double y, const double z) {
        info->setPosition(x, y, z);
    }

    /**
     * Sets rotation of the object to (rx,ry,rz)
     */
    void setRotation(const double rx, const double ry, const double rz) {
        info->setRotation(rx, ry, rz);
    }

    /**
     * Sets scale of the object to (sx,sy,sz)
     */
    void setScale(const double sx, const double sy, const double sz) {
        info->setScale(sx, sy, sz);
    }

    /**
     * Returns all the meshes
     * @return The vector of meshes
     */
    std::vector<Mesh> getMeshes() const {
        return mesh;
    }

    /**
     * Returns the current animation mesh
     * @return The current animation mesh
     */
    Mesh getMesh() const {
        return mesh[info->getCurrentAnimationStep()];
    }

    /**
     * Returns the bounding spheres
     * @return The bounding spheres
     */
    std::vector<bounding_sphere> getBoundingSpheres() const {
        return boundingSpheres;
    }

    /**
     * Returns the bounding sphere of the current animation mesh
     * @return The bounding sphere of the current animation mesh
     */
    bounding_sphere getBoundingSphere() const {
        return boundingSpheres[info->getCurrentAnimationStep()];
    }

    /**
     * Returns the shader
     * @return The shader
     */
    Shader getShader() const {
        return shader;
    }

    /**
     * Returns the textures
     * @return The textures
     */
    const std::tuple<Texture...> &getTextures() const {
        return textures;
    }

    /**
     * Returns the metadata of the object
     * @return The metadata of the object
     */
    ObjectInfo *getInfo() const {
        return info;
    }

    /**
     * Advances to the next animation frame
     */
    void nextAnimationFrame() {
        info->nextAnimationFrame();
    }
};

#endif //INC_3D_RENDERER_OBJECT_H
