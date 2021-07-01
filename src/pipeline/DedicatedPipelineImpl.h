//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_DEDICATEDPIPELINEIMPL_H
#define INC_3D_RENDERER_DEDICATEDPIPELINEIMPL_H

#include <cmath>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <vector>
#include "DedicatedPipeline.h"
#include "object/ObjectInfo.h"
#include "util/ChronoMeter.h"
#include "util/BoundingSphere.h"

struct bounding_box {
    double left;
    double right;
    double bottom;
    double top;
};


template<class target_t, class Mesh, class Vertex, class Shader, class ...Texture>
class DedicatedPipelineImpl : public DedicatedPipeline<target_t> {
private:
    std::vector<Mesh> mesh;
    Shader shader;
    std::tuple<Texture...> textures;
    bounding_sphere boundingSphere;
    ObjectInfo *info;
public:
    DedicatedPipelineImpl(std::vector<Mesh> &mesh, Shader &shader, const std::tuple<Texture...> &textures,
                          bounding_sphere boundingSphere, ObjectInfo *info)
            : mesh(
            mesh), shader(shader), textures(textures), boundingSphere(boundingSphere), info(info) {}


    // TODO Return bool to count rendered/culled objects
    void render(target_t *target, double *z_buffer, const unsigned int width, const unsigned int height,
                const std::array<double, 16> &viewMatrix,
                const std::array<double, 16> &projectionMatrix, const std::array<double, 16> &viewportMatrix) override {

        start_chrono(0);

        std::array<double, 16> combined = matrix_multiplication(projectionMatrix, matrix_multiplication(viewMatrix,
                                                                                                        info->getModelMatrix()));
        double left[4], right[4], bottom[4], top[4], near[4], far[4];
        for (int i = 0; i < 4; i++) {
            left[i] = combined[i + 12] + combined[i];
            right[i] = combined[i + 12] - combined[i];
            bottom[i] = combined[i + 12] + combined[i + 4];
            top[i] = combined[i + 12] - combined[i + 4];
            near[i] = combined[i + 12] + combined[i + 8];
            far[i] = combined[i + 12] - combined[i + 8];
        }

        double distanceL = signedDistance(left);
        double distanceR = signedDistance(right);
        double distanceB = signedDistance(bottom);
        double distanceT = signedDistance(top);
        double distanceN = signedDistance(near);
        double distanceF = signedDistance(far);

        if (distanceL < -boundingSphere.r || distanceR < -boundingSphere.r || distanceB < -boundingSphere.r ||
            distanceT < -boundingSphere.r || distanceN < -boundingSphere.r || distanceF < -boundingSphere.r)
            return;
        for (std::array<Vertex, 3> triangle : mesh[info->getCurrentAnimationStep()]) {
            start_chrono(7);
            start_chrono(3);
            /*
            // Model to world
            triangle[0].transform(info->getWorldMatrix());
            triangle[1].transform(info->getWorldMatrix());
            triangle[2].transform(info->getWorldMatrix());
            // World to view
            triangle[0].transform(viewMatrix);
            triangle[1].transform(viewMatrix);
            triangle[2].transform(viewMatrix);
            */
            /*
            Normalization
            double mag = sqrt(aL * aL + bL * bL + cL * cL);
            aL /= mag;
            bL /= mag;
            cL /= mag;
            dL /= mag;
            */


            // Projection
            double w1 = triangle[0].project(combined);
            double w2 = triangle[1].project(combined);
            double w3 = triangle[2].project(combined);
            // Clip to viewport
            triangle[0].viewportMapping(viewportMatrix);
            triangle[1].viewportMapping(viewportMatrix);
            triangle[2].viewportMapping(viewportMatrix);
            stop_chrono(3);

            bounding_box box = compute_box(triangle[0], triangle[1], triangle[2]);

            // Barycentric precomputations
            const Vertex &a = triangle[0];
            const Vertex &b = triangle[1];
            const Vertex &ca = triangle[2];

            double vaX = a.getX();
            double vaY = a.getY();
            double vbX = b.getX();
            double vbY = b.getY();
            double vcX = ca.getX();
            double vcY = ca.getY();

            double v0X = vbX - vaX;
            double v0Y = vbY - vaY;
            double v1X = vcX - vaX;
            double v1Y = vcY - vaY;

            double den = (v0X * v1Y - v1X * v0Y);
            double invDen = 1.0 / den;
#define GOBBOJ
#ifndef GOBBOJ
            new_renderer(triangle[0], triangle[1], triangle[2], width, height,
                         v0X, v0Y, v1X, v1Y, den, invDen, target, z_buffer, w1, w2, w3);
#else

            for (int r = std::max(0L,std::lround(box.top)); r < std::min((double)height, box.bottom); r++) {
                start_chrono(8);
                for (int c = std::max(0L, std::lround(box.left)); c < std::min((double)width, box.right); c++) {
                    if (r > 0 && r < height && c > 0 && c < width) {

                        double A1 = 1.0;
                        double A2 = 0;
                        double A3 = 0;
                        start_chrono(9);

                        double v2X = c - vaX;
                        double v2Y = r - vaY;

                        if (den != 0) {
                            A2 = (v2X * v1Y - v1X * v2Y) * invDen;
                            A3 = (v0X * v2Y - v2X * v0Y) * invDen;
                            A1 = 1.0 - A2 - A3;
                        }

                        stop_chrono(9);

                        start_chrono(6);
                        if (inside_test(A1, A2, A3)) {
                            // Z-Buffer testing
                            double z = 1.0 / (triangle[0].getZ() * (A1) + triangle[1].getZ() * (A2) +
                                              triangle[2].getZ() * (A3));

                            if (z < z_buffer[r * width + c]) {
                                start_chrono(4);
                                // Interpolates the vertex
                                Vertex interpolated = Vertex::interpolate(triangle[0], triangle[1], triangle[2], A1,
                                                                          A2, A3, w1, w2, w3);
                                stop_chrono(4);
                                start_chrono(5);
                                // Calls the fragment CharShader
                                target[r * width + c] = shader(interpolated, textures);
                                stop_chrono(5);
                                // Updates Z-Buffer
                                z_buffer[r * width + c] = z;
                            }
                        }
                        stop_chrono(6);
                    }
                    stop_chrono(8);
                }
            }
#endif

            //stop_chrono(7);
        }
        stop_chrono(0);
    }

private:

    double signedDistance(double plane[]) {
        return (plane[0] * boundingSphere.x + plane[1] * boundingSphere.y + plane[2] * boundingSphere.z + plane[3]) /
               sqrt(pow(plane[0], 2) + pow(plane[1], 2) + pow(plane[2], 2));
    }

    std::array<double, 16> matrix_multiplication(const std::array<double, 16> &a, const std::array<double, 16> &b) {
        std::array<double, 16> out{};
        out[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
        out[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
        out[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
        out[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

        out[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
        out[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
        out[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
        out[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

        out[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
        out[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
        out[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
        out[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

        out[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
        out[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
        out[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
        out[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
        return out;
    }

    /**
     * Computes the triangle area (for barycentric coordinates)
     *
     */
    inline double triangle_area(const Vertex &v1, const Vertex &v2, const double Ax, const double Ay) {
        double Bx = v1.getX();
        double By = v1.getY();
        double Cx = v2.getX();
        double Cy = v2.getY();

        return (Ax * (By - Cy) + Bx * (Cy - Ay) + Cx * (Ay - By)) / 2;
    }

    /**
     * Tests if pixel is inside a triangle
     *
     */
    inline bool inside_test(const double A1, const double A2, const double A3) {
        return (A1 >= 0 && A1 <= 1) && (A2 >= 0 && A2 <= 1) && (A3 >= 0 && A3 <= 1);
    }

    /**
    * Computes the bounding box
    *
    */
    inline bounding_box compute_box(const Vertex &v1, const Vertex &v2, const Vertex &v3) {
        std::pair<double, double> lr = std::minmax({v1.getX(), v2.getX(), v3.getX()});
        std::pair<double, double> tb = std::minmax({v1.getY(), v2.getY(), v3.getY()});
        return bounding_box({lr.first, lr.second, tb.second, tb.first});
    }


    inline void new_renderer(Vertex &v0, Vertex &v1, Vertex &v2, int width, int height,
                             double v0X,
                             double v0Y,
                             double v1X,
                             double v1Y,
                             double den,
                             double invDen,
                             target_t *target, double *z_buffer, double w1, double w2, double w3) {

        Vertex v0Orig = v0;
        Vertex v1Orig = (v1);
        Vertex v2Orig = (v2);

        // v0 ----> 3
        // v1 ----> 7

        // v0 ----> 7
        // v1 -----> 3

        order(v0, v1);
        order(v0, v2);
        order(v1, v2);

        int x0 = v0.getX();
        int x1 = v1.getX();
        int x2 = v2.getX();
        int y0 = v0.getY();
        int y1 = v1.getY();
        int y2 = v2.getY();
        if (y0 != y2 && (x0 != x1 || x1 != x2)) {
            double acDenominator = 1.0 / (y2 - y0);
            double acIncrement = (x2 - x0) * acDenominator;

            double abDenominator = 1.0 / (y1 - y0);
            double abIncrement = (x1 - x0) * abDenominator;

            double bcDenominator = 1.0 / (y2 - y1);
            double bcIncrement = (x2 - x1) * bcDenominator;

            double acOnBY = x0 + (y1 - y0) * acIncrement;
            bool bendOnLeft = acOnBY >= x1;
            double leftIncrement;
            double rightIncrement;
            if (bendOnLeft) {
                leftIncrement = abIncrement;
                rightIncrement = acIncrement;
            } else {
                leftIncrement = acIncrement;
                rightIncrement = abIncrement;
            }
            // x0= 588 x1 = 320 y0 = 103 y1 = 117  268/  14
            int yStart = std::max((int) y0, 0);
            double xLeft = (y0 == y1) ? x0 : x0 + (yStart - (int) y0) * leftIncrement;
            double xRight = (y0 == y1) ? x1 : x0 + (yStart - (int) y0) * rightIncrement;

            int yEnd = std::min((int) y2, height);
            int y1Int = (int) y1;
            for (int y = yStart; y < yEnd; y++) {
                if (y >= y1Int) {
                    if (bendOnLeft) {
                        leftIncrement = bcIncrement;
                    } else {
                        rightIncrement = bcIncrement;
                    }
                }
                int xStart = std::max((int) xLeft, 0);
                int xEnd = std::min((int) xRight, width);
                for (int x = xStart; x < xEnd; x++) {
                    //int targetIndex = y * width + x;
                    //target[targetIndex] = 0x000000;
                    // render
                    double A1 = 1.0;
                    double A2 = 0;
                    double A3 = 0;

                    double v2X = x - v0Orig.getX();
                    double v2Y = y - v0Orig.getY();

                    if (den != 0) {
                        A2 = (v2X * v1Y - v1X * v2Y) * invDen;
                        A3 = (v0X * v2Y - v2X * v0Y) * invDen;
                        A1 = 1.0 - A2 - A3;
                    }
                    // Z-Buffer testing
                    double z = 1.0 / (v0Orig.getZ() * (A1) + v1Orig.getZ() * (A2) +
                                      v2Orig.getZ() * (A3));
                    int targetIndex = y * width + x;
                    if (z < z_buffer[targetIndex]) {

                        // Interpolates the vertex
                        Vertex interpolated = Vertex::interpolate(v0Orig, v1Orig, v2Orig, A1,
                                                                  A2, A3, w1, w2, w3);

                        // Calls the fragment CharShader
                        target[targetIndex] = shader(interpolated, textures);

                        // Updates Z-Buffer
                        z_buffer[targetIndex] = z;
                    }
                }
                xLeft += leftIncrement;
                xRight += rightIncrement;
            }
        }
    }

    inline void order(Vertex &v0, Vertex &v1) {
        int x0 = v0.getX();
        int x1 = v1.getX();
        int y0 = v0.getY();
        int y1 = v1.getY();
        if (y0 > y1 || (y0 == y1 && x0 > x1)) {
            std::swap(v0, v1);
        }
    }
};

#endif //INC_3D_RENDERER_DEDICATEDPIPELINEIMPL_H
