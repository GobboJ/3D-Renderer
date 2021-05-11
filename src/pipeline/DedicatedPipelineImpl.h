//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_DEDICATEDPIPELINEIMPL_H
#define INC_3D_RENDERER_DEDICATEDPIPELINEIMPL_H

#include <cmath>
#include <tuple>
#include <iostream>
#include "DedicatedPipeline.h"
#include "object/ObjectInfo.h"
#include "util/ChronoMeter.h"

struct bounding_box {
    double left;
    double right;
    double bottom;
    double top;
};


template<class target_t, class Mesh, class Vertex, class Shader, class ...Texture>
class DedicatedPipelineImpl : public DedicatedPipeline<target_t> {
private:
    Mesh mesh;
    Shader shader;
    std::tuple<Texture...> textures;
    ObjectInfo *info;
public:
    DedicatedPipelineImpl(Mesh mesh, Shader shader, const std::tuple<Texture...> &textures, ObjectInfo *info)
            : mesh(
            mesh), shader(shader), textures(textures), info(info) {}


public:
    void render(target_t *target, double *z_buffer, const unsigned int width, const unsigned int height,
                const std::array<double, 16> &viewMatrix,
                const std::array<double, 16> &projectionMatrix, const std::array<double, 16> &viewportMatrix) override {

        start_chrono(0);
        for (std::array<Vertex, 3> triangle : mesh) {
            start_chrono(7);
            start_chrono(3);
            // Model to world
            triangle[0].transform(info->getWorldMatrix());
            triangle[1].transform(info->getWorldMatrix());
            triangle[2].transform(info->getWorldMatrix());
            // World to view
            triangle[0].transform(viewMatrix);
            triangle[1].transform(viewMatrix);
            triangle[2].transform(viewMatrix);
            // Projection
            double w1 = triangle[0].project(projectionMatrix);
            double w2 = triangle[1].project(projectionMatrix);
            double w3 = triangle[2].project(projectionMatrix);
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

            for (int r = std::lround(box.top); r < box.bottom; r++) {
                start_chrono(8);
                for (int c = std::lround(box.left); c < box.right; c++) {
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
            stop_chrono(7);
        }
        stop_chrono(0);
    }

private:

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
};

#endif //INC_3D_RENDERER_DEDICATEDPIPELINEIMPL_H
