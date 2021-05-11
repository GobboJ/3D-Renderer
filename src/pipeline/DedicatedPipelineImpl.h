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

class myEdge {
public:
    int x1, x2, y1, y2;
    myEdge(int x1, int y1, int x2, int y2, int width, int height) {

        //std::cout << y1 << " " << y2 << std::endl;
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

        this->x1 = (this->x1 < 0) ? 0 : this->x1;
        this->y1 = (this->y1 < 0) ? 0 : this->y1;
        this->x2 = (this->x2 > width-1) ? width-1 : this->x2;
        this->y2 = (this->y2 > height-1) ? height-1 : this->y2;
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





            bounding_box box = compute_box(triangle[0], triangle[1], triangle[2]);

            for (int r = std::lround(box.top); r < box.bottom; r++) {
                start_chrono(8);
                for (int c = std::lround(box.left); c < box.right; c++) {
                    if (r > 0 && r < height && c > 0 && c < width) {

                        double A1 = 1.0;
                        double A2 = 0;
                        double A3 = 0;
                        start_chrono(9);
                        //Barycentric(c, r, triangle[0], triangle[1], triangle[2], A1, A2, A3);

                        double v2X = c - vaX;
                        double v2Y = r - vaY;

                        if (den != 0) {
                            A2 = (v2X * v1Y - v1X * v2Y) * invDen;
                            A3 = (v0X * v2Y - v2X * v0Y) * invDen;
                            A1 = 1.0 - A2 - A3;
                        }

                        /*A1 = u;
                        A2 = v;
                        A3 = w;*/

                        stop_chrono(9);
                        start_chrono(2);
                        //double A1a = triangle_area(triangle[2], triangle[1], c, r);
                        //double A2a = triangle_area(triangle[0], triangle[2], c, r);
                        //double A3a = triangle_area(triangle[1], triangle[0], c, r);
                        stop_chrono(2);
/*
                        start_chrono(9);
                        Barycentric(c, r, triangle[0], triangle[1], triangle[2], A1, A2, A3);
                        stop_chrono(9);
                        start_chrono(10);
                        Barycentric2(c, r, triangle[0], triangle[1], triangle[2], A1, A2, A3);
                        stop_chrono(10);
                        double sum = 1;*/
/*
                        double a1bis = 3;
                        double a2bis = 3;
                        double a3bis = 3;

                        Barycentric2(c, r, triangle[0], triangle[1], triangle[2], a1bis, a2bis, a3bis);
                        double sum2 = a1bis + a2bis + a3bis;

                        double a2 = a1bis / sum2;
                        double b2 = a2bis / sum2;
                        double c2 = a3bis / sum2;


                        double a1tis = 3;
                        double a2tis = 3;
                        double a3tis = 3;

                        Barycentric(c, r, triangle[0], triangle[1], triangle[2], a1tis, a2tis, a3tis);
                        double sum3 = a1tis + a2tis + a3tis;

                        double a3 = a1tis / sum3;
                        double b3 = a2tis / sum3;
                        double c3 = a3tis / sum3;



                        double sum = A1 + A2 + A3;
                        double aCorr = A1 / sum;
                        double bCorr = A2 / sum;
                        double cCorr = A3 / sum;

                        if (a3tis > 0) {
                            int ssss = 3;
                        }
*/


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
                                target[r * width + c] = shader(interpolated);
                                // Updates Z-Buffer
                                z_buffer[r * width + c] = z;
                            }
                        }
                        stop_chrono(6);
                    }
                    stop_chrono(8);
                }
            }
            */
        }
        stop_chrono(0);
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

    void rasterize() {

    }
};

#endif //INC_3D_RENDERER_DEDICATEDPIPELINEIMPL_H
