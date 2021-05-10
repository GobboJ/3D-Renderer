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

        for (std::array<Vertex, 3> triangle : mesh) {
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

            /*
            bounding_box box = compute_box(triangle[0], triangle[1], triangle[2]);

            for (int r = std::lround(box.top); r < box.bottom; r++) {
                for (int c = std::lround(box.left); c < box.right; c++) {
                    if (r > 0 && r < height && c > 0 && c < width) {
                        // Computes barycentric coordinates
                        double A1 = triangle_area(triangle[2], triangle[1], c, r);
                        double A2 = triangle_area(triangle[0], triangle[2], c, r);
                        double A3 = triangle_area(triangle[1], triangle[0], c, r);
                        double sum = A1 + A2 + A3;

                        if (inside_test(A1, A2, A3, sum)) {
                            // Z-Buffer testing
                            double z = 1.0 / (triangle[0].getZ() * (A1/sum) + triangle[1].getZ() * (A2/sum) + triangle[2].getZ() * (A3/sum));

                            if (z < z_buffer[r * width + c]) {
                                // Interpolates the vertex
                                Vertex interpolated = Vertex::interpolate(triangle[0], triangle[1], triangle[2], A1,
                                                                          A2, A3, w1, w2, w3);
                                // Calls the fragment CharShader
                                target[r * width + c] = shader(interpolated);
                                // Updates Z-Buffer
                                z_buffer[r * width + c] = z;
                            }
                        }
                    }
                }
            }
            */

            Vertex &a = triangle[0];
            Vertex &b = triangle[1];
            Vertex &c = triangle[2];

            if (a.getY() > c.getY()) {
                std::swap(a, c);
            }

            if (a.getY() > b.getY()) {
                std::swap(a, b);
            }

            if (b.getY() > c.getY()) {
                std::swap(b, c);
            }

            bool isAbVertical = (a.getX() == b.getX());
            double m_ab = isAbVertical ? 0 : (a.getY() - b.getY()) / (a.getX() - b.getX());
            bool isBcVertical = (b.getX() == c.getX());
            double m_bc = isBcVertical ? 0 : (b.getY() - c.getY()) / (b.getX() - c.getX());
            bool isAcVertical = (a.getX() == c.getX());
            double m_ac = isAcVertical ? 0 : (a.getY() - c.getY()) / (a.getX() - c.getX());

            double q_ab = isAbVertical ? a.getX() : a.getY() - m_ab * a.getX();
            double q_bc = isBcVertical ? b.getX() : b.getY() - m_bc * b.getX();
            double q_ac = isAcVertical ? c.getX() : c.getY() - m_ac * c.getX();

            if (((b.getX() < a.getX() && a.getX() < c.getX()) || (b.getX() < c.getX() && c.getX() < a.getX()) || (c.getX() < b.getX() && b.getX() < a.getX()))) {
                if (a.getY() != b.getY()) {
                    render_line(a, b, c, z_buffer, target, w1, w2, w3, width, height, m_ab, q_ab, isAbVertical, m_ac, q_ac,
                                isAcVertical, a.getY(), b.getY());
                }
                if (b.getY() != c.getY()) {
                    render_line(a, b, c, z_buffer, target, w1, w2, w3, width, height, m_bc, q_bc, isBcVertical, m_ac,
                                q_ac, isAcVertical, b.getY(), c.getY());
                }
            } else {
                if (a.getY() != b.getY()) {
                    render_line(a, b, c, z_buffer, target, w1, w2, w3, width, height, m_ac, q_ac, isAcVertical, m_ab,
                                q_ab, isAbVertical, a.getY(), b.getY());
                }
                if (b.getY() != c.getY()) {
                    render_line(a, b, c, z_buffer, target, w1, w2, w3, width, height, m_ac, q_ac, isAcVertical, m_bc,
                                q_bc, isBcVertical, b.getY(), c.getY());
                }
            }

            /*
            if (triangle[0].getY() < triangle[1].getY()) {
                if (triangle[0].getY() < triangle[2].getY()) {
                    a = triangle[0];
                    if (triangle[1].getY() < triangle[2].getY()) {
                        b = triangle[1];
                        c = triangle[2];
                    } else {
                        b = triangle[2];
                        c = triangle[1];
                    }
                } else {
                    // 0 < 1
                    // 2 < 0
                    // 2 < 0 < 1
                    // triangle[2].getY() <= triangle[0].getY();
                    if (triangle[2].getY() < triangle[1].getY()) {
                        a = triangle[2];

                    }
                }
            }*/


        }
    }

private:


    void
    render_line(Vertex &a, Vertex &b, Vertex &c, double *z_buffer, target_t *target, double w1, double w2, double w3,
                int width, int height, double m_left, double q_left, bool isLeftVertical, double m_right,
                double q_right, bool isRightVertical,
                int min_y, int max_y) {


        min_y = std::max(min_y, 0);
        max_y = std::min(max_y, height-1);

        for (int y = min_y; y <= max_y; y++) {
            double computed_x_start = round((isLeftVertical) ? q_left : ((y - q_left) / m_left));
            double computed_x_end = round((isRightVertical) ? q_right : ((y - q_right) / m_right));

            if (computed_x_start > computed_x_end) {
                std::swap(computed_x_start, computed_x_end);
            }
            int x_start = std::max((int) computed_x_start, 0);
            int x_end = std::min((int) computed_x_end, width-1);

            for (int x = x_start; x <= x_end; x++) {
                const int targetCell = y * width + x;
                // Computes barycentric coordinates
                double A1 = triangle_area(c, b, x, y);
                double A2 = triangle_area(a, c, x, y);
                double A3 = triangle_area(b, a, x, y);
                double sum = A1 + A2 + A3;


                // Z-Buffer testing
                double z = 1.0 / (a.getZ() * (A1/sum) + b.getZ() * (A2/sum) + c.getZ() * (A3/sum));

                if (z < z_buffer[targetCell]) {
                    // Interpolates the vertex
                    Vertex interpolated = Vertex::interpolate(a, b, c, A1,
                                                              A2, A3, w1, w2, w3);
                    // Calls the fragment CharShader
                    target[targetCell] = shader(interpolated);
                    // Updates Z-Buffer
                    z_buffer[targetCell] = z;
                }

            }
        }
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
