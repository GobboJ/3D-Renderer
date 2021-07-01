//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_OBJECTINFO_H
#define INC_3D_RENDERER_OBJECTINFO_H

#include <array>
#include "../util/Vector3.h"

/**
 * Describes the metadata of an Object
 */
class ObjectInfo {

private:
    // Position of the object
    Vector3 position;
    // Rotation of the object
    Vector3 rotation;
    // Scale of the object
    Vector3 scale;
    // Current animation frame
    unsigned int animationSteps;
    // Total number of animation frames
    unsigned int currentAnimationStep;

    // Model matrix (Local -> World coordinates)
    std::array<double, 16> modelMatrix;

    /**
     * Computes the model matrix
     */
    void computeModelMatrix();

public:

    /**
     * Constructor of ObjectInfo
     * @param animationSteps Total number of animation frames
     * @param currentAnimationStep Starting animation step, defaults to 0
     * @param position Position of the object, defaults to (0,0,0)
     * @param rotation Rotation of the object, defaults to (0,0,0)
     * @param scale Scale of the object, defaults to (1,1,1)
     */
    explicit ObjectInfo(unsigned int animationSteps, unsigned int currentAnimationStep = 0,
                        const Vector3 &position = {0, 0, 0}, const Vector3 &rotation = {0, 0, 0},
                        const Vector3 &scale = {1, 1, 1});

    /**
     * Sets position of the object to (x,y,z)
     */
    void setPosition(double x, double y, double z);

    /**
     * Sets rotation of the object to (rx,ry,rz)
     */
    void setRotation(double rx, double ry, double rz);

    /**
     * Sets scale of the object to (sx,sy,sz)
     */
    void setScale(double sx, double sy, double sz);

    /**
     * Returns the position of the object
     */
    const Vector3 &getPosition() const;

    /**
     * Returns the rotation of the object
     */
    const Vector3 &getRotation() const;

    /**
     * Returns the scale of the object
     */
    const Vector3 &getScale() const;

    /**
     * Returns the current animation step
     */
    unsigned int getCurrentAnimationStep() const;

    /**
     * Sets the current animation step
     * @param animationStep Animation step to be set as current
     */
    void setCurrentAnimationStep(unsigned int animationStep);

    /**
     * Sets the next animation step as current
     */
    void nextAnimationFrame();

    /**
     * Returns the model matrix
     * @return
     */
    const std::array<double, 16> &getModelMatrix() const;

};

#endif //INC_3D_RENDERER_OBJECTINFO_H
