//
// Created by Jonathan on 09/04/2021.
//

#include "Camera.h"
#include <cmath>
#include <cassert>

Camera::Camera(double verticalFieldOfView, double nearPlane, double farPlane, const Vector3 &position,
               const Vector3 &target, const Vector3 &up_vector) : verticalFieldOfView(verticalFieldOfView),
                                                                  nearPlane(nearPlane), farPlane(farPlane) {
    assert (nearPlane >= 0);
    assert (farPlane >= 0 && nearPlane <= farPlane);

    Vector3 v = (position - target);
    v = v / v.norm();
    Vector3 r = (v.cross(up_vector));
    r = -r / r.norm();
    Vector3 u = v.cross(r);

    viewMatrix = {r.getX(), r.getY(), r.getZ(), -(position.dot(r)),
                  u.getX(), u.getY(), u.getZ(), -(position.dot(u)),
                  v.getX(), v.getY(), v.getZ(), -(position.dot(v)),
                  0, 0, 0, 1};



}

Camera::Camera(double verticalFieldOfView, double nearPlane, double farPlane, const Vector3 &position, double pitch,
               double yaw) {
    // TODO To be implemented
    throw 0;
}

double Camera::getVerticalFieldOfView() const {
    return verticalFieldOfView;
}

const std::array<double, 16> &Camera::getViewMatrix() const {
    return viewMatrix;
}

double Camera::getNearPlane() const {
    return nearPlane;
}

double Camera::getFarPlane() const {
    return farPlane;
}
