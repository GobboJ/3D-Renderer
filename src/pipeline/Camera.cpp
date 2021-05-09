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

    /*Vector3 v = (position - target); // f
    v = v / v.norm();
    Vector3 r = (v.cross(up_vector));
    //r = -r / r.norm();

    //Vector3 r = (up_vector.cross(v)); // l
    r = r / r.norm();

     /*
     Change "(c-l) / || c-l ||" to "(l-c) / || l-c ||".
     Change "r = -(v x u') / || v x u' ||" to "r = (v x u') / || v x u' ||", i.e., remove the minus sign.
     Change "u = v x r" to "u = r x v".
     Change "and $\mb v$ with $(0,0,1)$" to "and $\mb v$ with $(0,0,-1)$".
     In the left matrix that follows, change "v_x & v_y & v_z & 0" to "-v_x & -v_y & -v_z & 0".
     In the right matrix, change "v_x & v_y & v_z & -\mb t\cdot\mb v" to "-v_x & -v_y & -v_z & \mb t\cdot\mb v".\
     */

    //Vector3 u = v.cross(r); // u*/


    /******************************************************************************************
     Funzionano le posizioni ma i triangoli sono sbagliati

    Vector3 v = (target - position);
    v = v / v.norm();

    Vector3 r = (up_vector.cross(v));
    r = r / r.norm();

    Vector3 u = v.cross(r);

   //viewMatrix = {r.getX(), r.getY(), r.getZ(), -(position.dot(r)), /*-r.getX() * position.getX() - r.getY() * position.getY() - r.getZ() * position.getZ()*/
   //               u.getX(), u.getY(), u.getZ(), -(position.dot(u)), /*-u.getX() * position.getX() - u.getY() * position.getY() - u.getZ() * position.getZ(),//*/
   //              v.getX(), v.getY(), v.getZ(), -(position.dot(v)), /*-v.getX() * position.getX() - v.getY() * position.getY() - v.getZ() * position.getZ(),//*/
    //              0, 0, 0, 1};
    // ********************************************************************************************

    Vector3 v = (target - position);
    v = v / v.norm();

    Vector3 r = (v.cross(up_vector));
    r = r / r.norm();

    Vector3 u = v.cross(r);

    viewMatrix = {r.getX(), r.getY(), r.getZ(), -(position.dot(r)), /*-r.getX() * position.getX() - r.getY() * position.getY() - r.getZ() * position.getZ()*/
                  u.getX(), u.getY(), u.getZ(), -(position.dot(u)), /*-u.getX() * position.getX() - u.getY() * position.getY() - u.getZ() * position.getZ(),//*/
                  -v.getX(), -v.getY(), -v.getZ(), (position.dot(v)), /*-v.getX() * position.getX() - v.getY() * position.getY() - v.getZ() * position.getZ(),//*/
                  0, 0, 0, 1};

        /*
         * Soluzione del sito
    Vector3 f = position - target;
    f = f / f.norm();

    Vector3 l = f.cross(up_vector);
    l = l / l.norm();

    Vector3 u = l.cross(f);

    viewMatrix = {
            l.getX(), l.getY(), l.getZ(), -l.getX() * position.getX() - l.getY() * position.getY() - l.getZ() * position.getZ(),
            u.getY(), u.getY(), u.getZ(), -u.getX() * position.getX() - u.getY() * position.getY() - u.getZ() * position.getZ(),
            f.getX(), f.getY(), f.getZ(), -f.getX() * position.getX() - f.getY() * position.getY() - f.getZ() * position.getZ(),
            0, 0, 0, 1
    };
         */


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
