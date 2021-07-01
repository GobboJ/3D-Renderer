//
// Created by Jonathan on 09/04/2021.
//

#ifndef INC_3D_RENDERER_SIMPLEVERTEX_H
#define INC_3D_RENDERER_SIMPLEVERTEX_H

#include <array>
#include "../util/Vector3.h"

/**
 * Represents a SimpleVertex with a position (x,y,z) and a color
 */
class SimpleVertex {
private:
    // Coordinate x of the position
    double x;
    // Coordinate y of the position
    double y;
    // Coordinate z of the position
    double z;
    // Color of the vertex
    unsigned int color;

    /**
     * Multiplies the vertex by the row of the matrix specified by the index
     * @param matrix The transformation matrix
     * @param index The index of the row of the matrix
     * @return The result of the process.
     */
    inline double multiply(const std::array<double, 16> &matrix, int index) const;

public:

    /**
     * Constructor of SimpleVertex
     * @param x Coordinate x of the position
     * @param y Coordinate y of the position
     * @param z Coordinate z of the position
     * @param color Color
     */
    SimpleVertex(double x, double y, double z, unsigned int color);

    /**
     * Returns the coordinate x
     */
    double getX() const;

    /**
     * Returns the coordinate y
     */
    double getY() const;

    /**
     * Returns the coordinate z
     */
    double getZ() const;

    /**
     * Returns the color
     */
    unsigned int getColor() const;

    /**
     * Computes the distance between the vertices this and vertex
     * @param vertex The vertex to compute the distance from.
     * @return The distance between the two vertices
     */
    double distance(SimpleVertex &vertex) const;

    /**
     * Applies a transformation matrix to the vertex
     * @param matrix The transformation matrix
     */
    void transform(const std::array<double, 16> &matrix);

    /**
     * Applies the projection matrix to the vertex
     * @param projectionMatrix The projection matrix
     * @return The value of w produced by the operation.
     */
    double project(const std::array<double, 16> &projectionMatrix);

    /**
     * Applies the viewport matrix to the vertex
     * @param viewportMatrix The viewport matrix
     */
    void viewportMapping(const std::array<double, 16> &viewportMatrix);

    /**
     * Returns the resulting SimpleVertex from the interpolation of v1, v2, v3,
     * using the barycentric coordinates A1, A2, A3 and the w components w0, w1, w2
     * @param v1 First vertex
     * @param v2 Second vertex
     * @param v3 Third vertex
     * @param A1 First barycentric coordinate
     * @param A2 Second barycentric coordinate
     * @param A3 Third barycentric coordinate
     * @param w0 First w component
     * @param w1 Second w component
     * @param w2 Third w component
     * @return The interpolated vertex
     */
    static SimpleVertex
    interpolate(const SimpleVertex &v1, const SimpleVertex &v2, const SimpleVertex &v3, double A1,
                double A2, double A3, double w0, double w1, double w2);

};

#endif //INC_3D_RENDERER_SIMPLEVERTEX_H
