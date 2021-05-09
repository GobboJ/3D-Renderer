//
// Created by Jonathan on 09/04/2021.
//

#include "Camera.h"
#include <cassert>

Camera::Camera(const double verticalFieldOfView, const double nearPlane, const double farPlane, const Vector3 &position,
               const Vector3 &target, const Vector3 &up_vector) : verticalFieldOfView(verticalFieldOfView),
                                                                  nearPlane(nearPlane), farPlane(farPlane) {
    assert (nearPlane >= 0);
    assert (farPlane >= 0 && nearPlane <= farPlane);

    Vector3 v = (target - position);
    v = v / v.norm();

    Vector3 r = (v.cross(up_vector));
    r = r / r.norm();

    Vector3 u = v.cross(r);

    viewMatrix = {r.getX(), r.getY(), r.getZ(), -(position.dot(r)),
                  u.getX(), u.getY(), u.getZ(), -(position.dot(u)),
                  -v.getX(), -v.getY(), -v.getZ(), (position.dot(v)),
                  0, 0, 0, 1};

}

Camera::Camera(const double verticalFieldOfView, const double nearPlane, const double farPlane, const Vector3 &position,
               const double pitch,
               const double yaw) {
    // TODO To be implemented
    assert(false);
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
