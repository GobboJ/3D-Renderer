//
// Created by Jonathan on 10/05/2021.
//

#ifndef INC_3D_RENDERER_TEXTUREVERTEX_H
#define INC_3D_RENDERER_TEXTUREVERTEX_H


#include <array>

/**
 * Represents a SimpleVertex with a position (x,y,z) and texture coordinates (u,v)
 */
class TextureVertex {
private:
    // Coordinate x of the position
    double x;
    // Coordinate y of the position
    double y;
    // Coordinate z of the position
    double z;
    // Coordinate u of the texture
    double u;
    // Coordinate v of the texture
    double v;

    /**
     * Multiplies the vertex by the row of the matrix specified by the index
     * @param matrix The transformation matrix
     * @param index The index of the row of the matrix
     * @return The result of the process.
     */
    inline double multiply(const std::array<double, 16> &matrix, int index) const;

public:
    TextureVertex(double x, double y, double z, double u, double v);

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
     * Returns the coordinate u
     */
    double getU() const;

    /**
     * Returns the coordinate v
     */
    double getV() const;

    /**
     * Computes the distance between the vertices this and vertex
     * @param vertex The vertex to compute the distance from.
     * @return The distance between the two vertices
     */
    double distance(TextureVertex &vertex) const;

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
     * Returns the resulting TextureVertex from the interpolation of v1, v2, v3,
     * using the barycentring coordinates A1, A2, A3 and the w components w0, w1, w2
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
    static TextureVertex
    interpolate(const TextureVertex &v1, const TextureVertex &v2, const TextureVertex &v3, double A1,
                double A2, double A3, double w0, double w1, double w2);

};


#endif //INC_3D_RENDERER_TEXTUREVERTEX_H
