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

class myEdge {
public:
    int x1, x2, y1, y2;
    myEdge(int x1, int y1, int x2, int y2, int width, int height) {
        x1 = (x1 < 0) ? 0 : x1;
        y1 = (y1 < 0) ? 0 : y1;
        x2 = (x2 > width-1) ? width-1 : x2;
        y2 = (y2 > height-1) ? height-1 : y2;
        if (y1 < y2) {
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;
        } else {
            this->x1 = x2;
            this->y1 = y2;
            this->x2 = x1;
            this->y2 = y1;
        }
    }
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
            Rasterizer<target_t> rast = {target, width, height};
            rast.drawTriangle(0, triangle[0].getX(), triangle[0].getY(), 0, triangle[1].getX(), triangle[1].getY(), 0,
                              triangle[2].getX(), triangle[2].getY());
            */


            /*Edge firstEdge(0, triangle[0].getX(), triangle[0].getY(), 0, triangle[1].getX(), triangle[1].getY());
            Edge secondEdge(0, triangle[1].getX(), triangle[1].getY(), 0, triangle[2].getX(), triangle[2].getY());
            Edge thirdEdge(0, triangle[0].getX(), triangle[0].getY(), 0, triangle[2].getX(), triangle[2].getY());

            int maxLength = firstEdge.Y2 - firstEdge.Y1;
            Edge tallest = firstEdge;
            Edge shortOne = secondEdge;
            Edge shortTwo = thirdEdge;

            int secondEdgeLength = secondEdge.Y2 - secondEdge.Y1;
            int thirdEdgeLength = thirdEdge.Y2 - thirdEdge.Y1;
            if (secondEdgeLength > maxLength) {
                maxLength = secondEdgeLength;
                tallest = secondEdge;
                shortOne = firstEdge;
            }
            if (thirdEdgeLength > maxLength) {
                maxLength = thirdEdgeLength;
                shortTwo = tallest;
                tallest = thirdEdge;
            }
             Rasterizer<target_t> r(target, width, height);
            r.drawSpans(tallest, shortOne);
            r.drawSpans(tallest, shortTwo);
             */

            myEdge firstEdge(triangle[0].getX(), triangle[0].getY(), triangle[1].getX(), triangle[1].getY(), width, height);
            myEdge secondEdge(triangle[1].getX(), triangle[1].getY(), triangle[2].getX(), triangle[2].getY(), width, height);
            myEdge thirdEdge(triangle[0].getX(), triangle[0].getY(), triangle[2].getX(), triangle[2].getY(), width, height);

            int maxLength = firstEdge.y2 - firstEdge.y1;
            myEdge tallest = firstEdge;
            myEdge shortOne = secondEdge;
            myEdge shortTwo = thirdEdge;

            int secondEdgeLength = secondEdge.y2 - secondEdge.y1;
            int thirdEdgeLength = thirdEdge.y2 - thirdEdge.y1;
            if (secondEdgeLength > maxLength) {
                maxLength = secondEdgeLength;
                tallest = secondEdge;
                shortOne = firstEdge;
            }
            if (thirdEdgeLength > maxLength) {
                maxLength = thirdEdgeLength;
                shortTwo = tallest;
                tallest = thirdEdge;
            }

            draw_line(tallest, shortOne, triangle[0], triangle[1], triangle[2], w1, w2, w3, target, z_buffer, width, height);
            draw_line(tallest, shortTwo, triangle[0], triangle[1], triangle[2], w1, w2, w3, target, z_buffer, width, height);




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
        }
    }

private:

    void draw_line(myEdge &tallestEdge, myEdge &shortEdge, Vertex &v0, Vertex &v1, Vertex &v2, double w1, double w2, double w3, target_t *target, double* z_buffer, int width, int height) {

        if ((tallestEdge.y2 - tallestEdge.y1) != 0 && (shortEdge.y2 - shortEdge.y1) != 0){

            double tallestEdgeWidth = tallestEdge.x2 - tallestEdge.x1;
            double shortEdgeWidth = shortEdge.x2 - shortEdge.x1;
            double firstFactor = ((double ) (shortEdge.y1 - tallestEdge.y1)) / (tallestEdge.y2 - tallestEdge.y1);
            double firstFactorStep = 1.0 / (tallestEdge.y2 - tallestEdge.y1);
            double secondFactor = 0;
            double secondFactorStep = 1.0 / (shortEdge.y2 - shortEdge.y1);

            for (int y = shortEdge.y1; y < shortEdge.y2; y++) {

                int x_start = tallestEdge.x1 + (int) (tallestEdgeWidth * firstFactor);
                int x_end = shortEdge.x1 + (int) (shortEdgeWidth * secondFactor);
                if (x_start > x_end) {
                    int tmp = x_start;
                    x_start = x_end;
                    x_end = tmp;
                }
                if ((x_end - x_start) != 0) {
                    for (int x = x_start; x < x_end; x++) {
                        // Computes barycentric coordinates
                        double A1 = triangle_area(v2, v1, x, y);
                        double A2 = triangle_area(v0, v2, x, y);
                        double A3 = triangle_area(v1, v0, x, y);
                        double sum = A1 + A2 + A3;
                        // Z-Buffer testing
                        double z = 1.0 / (v0.getZ() * (A1 / sum) + v1.getZ() * (A2 / sum) + v2.getZ() * (A3 / sum));

                        if (z < z_buffer[y * width + x]) {

                            // sheidazione
                            Vertex interpolated = Vertex::interpolate(v0, v1, v2, A1,
                                                                      A2, A3, w1, w2, w3);
                            // Calls the fragment CharShader
                            target[y * width + x] = shader(interpolated);
                            // Updates Z-Buffer
                            z_buffer[y * width + x] = z;

                        }
                    }
                }
                firstFactor += firstFactorStep;
                secondFactor += secondFactorStep;
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
