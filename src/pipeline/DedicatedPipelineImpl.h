//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_DEDICATEDPIPELINEIMPL_H
#define INC_3D_RENDERER_DEDICATEDPIPELINEIMPL_H

#include <tuple>
#include <iostream>
#include "DedicatedPipeline.h"
#include "object/ObjectInfo.h"

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

    void printTriag(Vertex v) {
        std::cout << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
    }

public:
    void render(target_t *target, double *z_buffer, const unsigned int width, const unsigned int height,
                const std::array<double, 16> &viewMatrix,
                const std::array<double, 16> &projectionMatrix, const std::array<double, 16> &viewportMatrix) override {

        /*for (std::array<Vertex, 3> triangle : mesh) {
            std::cout << "[";
            printTriag(triangle[0]);
            std::cout << ", ";
            printTriag(triangle[1]);
            std::cout << ", ";
            printTriag(triangle[2]);
            std::cout << "]" << std::endl;
        }
        std::cout << std::endl;*/

        for (std::array<Vertex, 3> triangle : mesh) {
            // Model to world
            //std::cout << "[";
            //printTriag(triangle[0]);

            triangle[0].transform(info->getWorldMatrix());

            //std::cout << " --> ";
            //printTriag(triangle[0]);

            //std::cout << ", ";
            //printTriag(triangle[1]);
            triangle[1].transform(info->getWorldMatrix());

            //std::cout << " --> ";
            //printTriag(triangle[1]);
            //std::cout << ", ";
            //printTriag(triangle[2]);

            triangle[2].transform(info->getWorldMatrix());


            /*std::cout << " --> ";
            printTriag(triangle[2]);
            std::cout << "]" << std::endl;*/

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

            bounding_box box = compute_box(triangle[0], triangle[1], triangle[2]);

            for (int r = std::lround(box.top); r < box.bottom; r++) {
                for (int c = std::lround(box.left); c < box.right; c++) {
                    if (r > 0 && r < height && c > 0 && c < width) {
                        // Computes barycentric coordinates
                        double A1 = 0.5 * triangle_area(triangle[2], triangle[1], c, r);
                        double A2 = 0.5 * triangle_area(triangle[0], triangle[2], c, r);
                        double A3 = 0.5 * triangle_area(triangle[1], triangle[0], c, r);
                        double sum = A1 + A2 + A3;

                        if (inside_test(A1, A2, A3, sum)) {
                            // Z-Buffer testing
                            double z = 1.0 / (triangle[0].getZ() * A1 + triangle[1].getZ() * A2 + triangle[2].getZ() * A3);

                            if (z < z_buffer[r * width + c]) {
                                // Interpolates the vertex
                                Vertex interpolated = Vertex::interpolate(triangle[0], triangle[1], triangle[2], A1,
                                                                          A2, A3, w1, w2, w3);
                                // Calls the fragment SimpleShader
                                target[r * width + c] = shader(interpolated);
                                // Updates Z-Buffer
                                z_buffer[r * width + c] = z;
                            }
                        }
                    }
                }
            }

            int a = 1;
        }
        int a = 3;
    }

private:
    /**
     * Computes the triangle area (for barycentric coordinates)
     *
     */
    inline double triangle_area(const Vertex &v1, const Vertex &v2, const double x, const double y) {
        return ((v1.getY() - v2.getY()) * x - (v1.getX() - v2.getX()) * y + v1.getX() * v2.getY() -
                v2.getX() * v1.getY());
    }

    /**
     * Tests if pixel is inside a triangle
     *
     */
    inline bool inside_test(const double A1, const double A2, const double A3, const double sum) {
        return (A1 / sum >= 0 && A1 / sum <= 1) && (A2 / sum >= 0 && A2 / sum <= 1) && (A3 / sum >= 0 && A3 / sum <= 1);
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

    void rasterize() {

    }
};

#endif //INC_3D_RENDERER_DEDICATEDPIPELINEIMPL_H
