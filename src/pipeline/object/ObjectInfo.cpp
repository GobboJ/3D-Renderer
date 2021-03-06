//
// Created by Jonathan on 06/05/2021.
//

#include "ObjectInfo.h"
#include <cmath>

ObjectInfo::ObjectInfo(unsigned int animationSteps, unsigned int currentAnimationStep, const Vector3 &position,
                       const Vector3 &rotation, const Vector3 &scale) : position(position),
                                                                        rotation(rotation),
                                                                        scale(scale),
                                                                        animationSteps(animationSteps),
                                                                        currentAnimationStep(currentAnimationStep),
                                                                        modelMatrix({1, 0, 0, 0,
                                                                                     0, 1, 0, 0,
                                                                                     0, 0, 1, 0,
                                                                                     0, 0, 0, 1}) {}

void ObjectInfo::setPosition(const double x, const double y, const double z) {
    ObjectInfo::position = {x, y, z};
    computeModelMatrix();
}

void ObjectInfo::setRotation(const double rx, const double ry, const double rz) {
    ObjectInfo::rotation = {rx, ry, rz};
    computeModelMatrix();
}

void ObjectInfo::setScale(const double sx, const double sy, const double sz) {
    ObjectInfo::scale = {sx, sy, sz};
    computeModelMatrix();
}

const Vector3 &ObjectInfo::getPosition() const {
    return position;
}

const Vector3 &ObjectInfo::getRotation() const {
    return rotation;
}

const Vector3 &ObjectInfo::getScale() const {
    return scale;
}

const std::array<double, 16> &ObjectInfo::getModelMatrix() const {
    return modelMatrix;
}

void ObjectInfo::computeModelMatrix() {
    const double CONV = 3.1415926535897932384626433832795 / 180;

    double tx = position.getX();
    double ty = position.getY();
    double tz = position.getZ();

    double rx = rotation.getX() * CONV;
    double ry = rotation.getY() * CONV;
    double rz = rotation.getZ() * CONV;

    double sx = scale.getX();
    double sy = scale.getY();
    double sz = scale.getZ();

    double sin_rx = sin(rx);
    double sin_ry = sin(ry);
    double sin_rz = sin(rz);
    double cos_rx = cos(rx);
    double cos_ry = cos(ry);
    double cos_rz = cos(rz);

    // Computes the model matrix
    modelMatrix = {
            sx * cos_ry * cos_rz, -sy * cos_ry * sin_rz, sz * sin_ry, tx,
            sx * (sin_rx * sin_ry * cos_rz + cos_rx * sin_rz),
            sy * (cos_rx * cos_rz - sin_rx * sin_ry * sin_rz), -sz * sin_rx * cos_ry, ty,
            sx * (sin_rx * sin_rz - cos_rx * sin_ry * cos_rz),
            sy * (cos_rx * sin_ry * sin_rz + sin_rx * cos_rz), sz * cos_rx * cos_ry, tz,
            0, 0, 0, 1
    };
}

unsigned int ObjectInfo::getCurrentAnimationStep() const {
    return currentAnimationStep;
}

void ObjectInfo::setCurrentAnimationStep(unsigned int animationStep) {
    if (animationStep < animationSteps)
        ObjectInfo::currentAnimationStep = animationStep;
    else
        ObjectInfo::currentAnimationStep = animationSteps;
}

void ObjectInfo::nextAnimationFrame() {
    currentAnimationStep = (currentAnimationStep + 1) % animationSteps;
}



