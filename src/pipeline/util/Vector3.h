//
// Created by Jonathan on 16/04/2021.
//

#ifndef INC_3D_RENDERER_VECTOR3_H
#define INC_3D_RENDERER_VECTOR3_H

#include <cmath>

/**
 * Represents a three dimensional vector
 */
class Vector3 {
private:
    // First component
    double x;
    // Second component
    double y;
    // Third component
    double z;

public:

    /**
     * Constructor of Vector3
     * @param x First component
     * @param y Second component
     * @param z Third component
     */
    Vector3(double x, double y, double z);

    /**
     * Returns the first component of the vector
     */
    double getX() const;

    /**
     * Returns the second component of the vector
     */
    double getY() const;

    /**
     * Returns the third component of the vector
     */
    double getZ() const;

    /**
     * Computes the norm of the vector
     */
    double norm() const;

    /**
     * Computes the dot product between the vectors this and v
     * @param v The other Vector3 to compute the dot product with.
     * @return The dot product.
     */
    double dot(const Vector3 &v) const;

    /**
     * Computes the cross product between the vectors this and v
     * @param v The other Vector3 to compute the cross product with.
     * @return The Vector3 representing the cross product.
     */
    Vector3 cross(const Vector3 &v) const;

};


inline Vector3 Vector3::cross(const Vector3 &v) const {
    return {y * v.getZ() - z * v.getY(),
            z * v.getX() - x * v.getZ(),
            x * v.getY() - y * v.getX()};
}

inline double Vector3::dot(const Vector3 &v) const {
    return x * v.getX() + y * v.getY() + z * v.getZ();
}

inline double Vector3::norm() const {
    return sqrt(x * x + y * y + z * z);
}

/**
 * Computes the scalar product between t and v
 * @param t Scalar
 * @param v Vector3
 * @return The Vector3 representing the result.
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
 * Computes the scalar division between t and v
 * @param t Scalar
 * @param v Vector3
 * @return The Vector3 representing the result.
 */
inline Vector3 operator/(Vector3 v, double t) {
    return (1 / t) * v;
}

/**
 * Computes the sum between vectors u and v
 * @param u First Vector3
 * @param v Second Vector3
 * @return The Vector3 representing the result.
 */
inline Vector3 operator+(const Vector3 &u, const Vector3 &v) {
    return {u.getX() + v.getX(), u.getY() + v.getY(), u.getZ() + v.getZ()};
}

/**
 * Computes the unary minus of vector u
 * @param u Vector3
 * @return The Vector3 representing the result.
 */
inline Vector3 operator-(const Vector3 &u) {
    return {-u.getX(), -u.getY(), -u.getZ()};
}

/**
 * Computes the difference between vectors u and v
 * @param u First Vector3
 * @param v Second Vector3
 * @return The Vector3 representing the result
 */
inline Vector3 operator-(const Vector3 &u, const Vector3 &v) {
    return u + (-v);
}




#endif //INC_3D_RENDERER_VECTOR3_H
