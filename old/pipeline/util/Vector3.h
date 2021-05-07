//
// Created by Jonathan on 16/04/2021.
//

#ifndef INC_3D_RENDERER_VECTOR3_H
#define INC_3D_RENDERER_VECTOR3_H

#include <cmath>

class Vector3 {
private:
    double x;
    double y;
    double z;
public:
    double getX() const;

    double getY() const;

    double getZ() const;

public:
    Vector3(double x, double y, double z);

    double norm() const;

    double dot(const Vector3 &v) const;

    Vector3 cross(const Vector3 &v) const;
};


/**
 * Scalar product
 * @param t
 * @param v
 * @return
 */
inline Vector3 operator*(double t, const Vector3 &v) {
    return {t * v.getX(), t * v.getY(), t * v.getZ()};
}

/*
inline Vector3 operator*(const Vector3 &v, double t) {
    return t * v;
}
*/

/**
 * Cross product
 * @param v
 * @return
 */
inline Vector3 Vector3::cross(const Vector3 &v) const {
    return {y * v.getZ() - z * v.getY(),
            z * v.getX() - x * v.getZ(),
            x * v.getY() - y * v.getX()};
}

/**
 * Dot product
 * @param v
 * @return
 */
inline double Vector3::dot(const Vector3 &v) const {
    return x * v.getX() + y * getY() + z * getZ();
}


/**
 * Scalar division
 * @param v
 * @param t
 * @return
 */
inline Vector3 operator/(Vector3 v, double t) {
    return (1 / t) * v;
}

/**
 * Sum
 * @param u
 * @param v
 * @return
 */
inline Vector3 operator+(const Vector3 &u, const Vector3 &v) {
    return {u.getX() + v.getX(), u.getY() + v.getY(), u.getZ() + v.getZ()};
}

/**
 * Unary minus
 * @param u
 * @return
 */
inline Vector3 operator-(const Vector3 &u) {
    return {-u.getX(), -u.getY(), -u.getZ()};
}

/**
 * Difference
 * @param u
 * @param v
 * @return
 */
inline Vector3 operator-(const Vector3 &u, const Vector3 &v) {
    return u + (-v);
}

/**
 * Norm of the vector
 * @return
 */
inline double Vector3::norm() const {
    return sqrt(x * x + y * y + z * z);
}


#endif //INC_3D_RENDERER_VECTOR3_H
