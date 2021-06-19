//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_OBJECTINFO_H
#define INC_3D_RENDERER_OBJECTINFO_H


#include <array>
#include "../util/Vector3.h"

class ObjectInfo {
private:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    std::array<double, 16> worldMatrix;
    unsigned int animationSteps;
    unsigned int currentAnimationStep;
public:

    explicit ObjectInfo(unsigned int animationSteps, unsigned int currentAnimationStep = 0,
                        const Vector3 &position = {0, 0, 0}, const Vector3 &rotation = {0, 0, 0},
                        const Vector3 &scale = {1, 1, 1});

    void setPosition(double x, double y, double z);

    void setRotation(double rx, double ry, double rz);

    void setScale(double sx, double sy, double sz);

    const Vector3 &getPosition() const;

    const Vector3 &getRotation() const;

    const Vector3 &getScale() const;

    unsigned int getCurrentAnimationStep() const;

    void setCurrentAnimationStep(unsigned int currentAnimationStep);

    void nextAnimationFrame();

    const std::array<double, 16> &getWorldMatrix() const;

private:
    void computeWorldMatrix();
};

#endif //INC_3D_RENDERER_OBJECTINFO_H
