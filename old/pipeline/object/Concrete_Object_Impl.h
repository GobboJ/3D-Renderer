//
// Created by Jonathan on 09/04/2021.
//
#include "../util/Vector3.h"

// For Windows
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <cmath>

#ifndef INC_3D_RENDERER_CONCRETE_OBJECT_IMPL_H
#define INC_3D_RENDERER_CONCRETE_OBJECT_IMPL_H




namespace Object_private {

    struct bounding_box{
        double left;
        double right;
        double bottom;
        double top;
    };

    template<class M, class S, class ...T>
    class Concrete_Object_Impl : public Object_Impl {

    private:
        M mesh;
        S shader;
        std::tuple<T...> textures;
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
        std::array<double, 16> world;

    public:

        Concrete_Object_Impl(M &&mesh, S &&shader) : mesh(std::forward<M>(mesh)),
                                                     shader(std::forward<S>(shader)),
                                                     textures(std::forward<T>(textures) ...),
                                                     position(0, 0, 0),
                                                     rotation(0, 0, 0),
                                                     scale(1, 1, 1),
                                                     world({1., 0., 0., 0.,
                                                            0., 1., 0., 0.,
                                                            0., 0., 1., 0.,
                                                            0., 0., 0., 1.}) {

        }

        /*
        void computeWorldMatrix() {

            double CONV = M_PI / 180;

            double sx = scale.getX();
            double sy = scale.getY();
            double sz = scale.getZ();
            double rx = rotation.getX() * CONV;
            double ry = rotation.getY() * CONV;
            double rz = rotation.getZ() * CONV;
            double tx = position.getX();
            double ty = position.getY();
            double tz = position.getZ();

            world = {
                    sx * cos(ry) * cos(rz), sy * (sin(rx) * sin(ry) * cos(rz) - cos(rx) * sin(rz)),
                    sz * (cos(rx) * sin(ry) * cos(rz) + sin(rx) * sin(rz)), tx,
                    sx * cos(ry) * sin(rz), sy * (cos(rx) * cos(rz) + sin(rx) * sin(ry) * sin(rz)),
                    sz * (cos(rx) * sin(ry) * sin(rz) - sin(rx) * cos(rz)), ty,
                    -sx * sin(ry), sy * sin(rx) * cos(ry), sz * cos(rx) * cos(ry), tz,
                    0, 0, 0, 1
            };
        }
        */
        void computeWorldMatrix() {
            const double CONV = M_PI / 180;

            double sx = scale.getX();
            double sy = scale.getY();
            double sz = scale.getZ();
            double rx = rotation.getX() * CONV;
            double ry = rotation.getY() * CONV;
            double rz = rotation.getZ() * CONV;
            double sin_rx = sin(rx);
            double sin_ry = sin(ry);
            double sin_rz = sin(rz);
            double cos_rx = cos(rx);
            double cos_ry = cos(ry);
            double cos_rz = cos(rz);
            double tx = position.getX();
            double ty = position.getY();
            double tz = position.getZ();

            /*world = {
                    sx * cos(ry) * cos(rz), sy * (sin(rx) * sin(ry) * cos(rz) + cos(rx) * sin(rz)),
                    sz * (-cos(rx) * sin(ry) * cos(rz) + sin(rx) * sin(rz)), tx,
                    sx * -cos(ry) * sin(rz), sy * (cos(rx) * cos(rz) - sin(rx) * sin(ry) * sin(rz)),
                    sz * (cos(rx) * sin(ry) * sin(rz) + sin(rx) * cos(rz)), ty,
                    -sx * sin(ry), sy * -sin(rx) * cos(ry), sz * cos(rx) * cos(ry), tz,
                    0, 0, 0, 1
            };*/
            world = {
                    sx * cos_ry * cos_rz, -sy * cos_ry * sin_rz, sz * sin_ry, tx,
                    sx * (sin_rx * sin_ry * cos_rz + cos_rx * sin_rz),
                    sy * (cos_rx * cos_rz - sin_rx * sin_ry * sin_rz), -sz * sin_rx * cos_ry, ty,
                    sx * (sin_rx * sin_rz - cos_rx * sin_ry * cos_rz),
                    sy * (cos_rx * sin_ry * sin_rz + sin_rx * cos_rz), sz * cos_rx * cos_ry, tz,
                    0, 0, 0, 1
            };
        }

        void setPosition(const double x, const double y, const double z) override {
            position = {x, y, z};
            computeWorldMatrix();
        }

        void setRotation(const double rot_x, const double rot_y, const double rot_z) override {
            rotation = {rot_x, rot_y, rot_z};
            computeWorldMatrix();
        }

        void setScale(const double scale_x, const double scale_y, const double scale_z) override {
            scale = {scale_x, scale_y, scale_z};
            computeWorldMatrix();
        }



        /*
            a, b, c = scala
            // ----> d, e, f = angoli
            d, f, i = cos(alpha)
            e, g, h = sin(alpha)
            j, k, l = traslazione

            [[1,0,0,j], [0,1,0,k], [0,0,1,l], [0,0,0,1]] * 
            [[i, -h,0,0], [h,i,0,0],[0,0,1,0],[0,0,0,1]] *
            [[f,0,g,0],[0,1,0,0],[-g,0,f,0],[0,0,0,1]] * 
            [[1,0,0,0], [0,d,-e,0], [0,e,d,0],[0,0,0,1]]  *
            [[a,0,0,0],[0,b,0,0],[0,0,c,0],[0,0,0,1]]

        */

        /*
         * vtxs = [(1, 1, 1), (2, 2, 2), (3,3,3), (4,4,4)]
         * trg1 = {&vtxs, [1, 2, 3]}
         * trg2 = {&vtxs, [2, 3, 4]}
         * t3g2 = {&vtxs, [1, 3, 4]}
         *
         * indices = [1,2,3,2,3,4,1,3,4]
         *
         * vtxs = [(1, 1, 1), (2, 2, 2), (3,3,3), (4,4,4)]
         * trg1 = [&vtxs[0], &vtxs[1], &vtx[2]]
         * trg2 = [&vtxs[1], &vtxs[2], &vtx[3]]
         * trg3 = [&vtxs[0], &vtxs[2], &vtx[3]]
         *
         * indices = [&vtxs[0], &vtxs[1], &vtx[2], &vtxs[1], &vtxs[2], &vtx[3], &vtxs[0], &vtxs[2], &vtxs[3]]
         *
         * vtxs[indeces[0]].x ...
         * indeces[0].x ...
         */

        void viewportMapping(const std::array<double, 16> &viewportMatrix, std::array<double, 3> &v) {
            v[0] = v[0] * viewportMatrix[0] + viewportMatrix[3];
            v[1] = v[1] * viewportMatrix[5] + viewportMatrix[7];
            v[2] = v[2] * viewportMatrix[10] + viewportMatrix[11];
        }

        /**
* Computes the bounding box
*
*/
        bounding_box compute_box(const std::array<double, 3> &v1, const std::array<double, 3> &v2, const std::array<double, 3> &v3) {
            std::pair<double, double> lr = std::minmax({v1[0], v2[0], v3[0]});
            std::pair<double, double> tb = std::minmax({v1[1], v2[1], v3[1]});
            return bounding_box({lr.first, lr.second, tb.second, tb.first});
        }

/**
 * Computes the triangle area (for barycentric coordinates)
 *
 */
        double triangle_area(const std::array<double, 3> &v1, const std::array<double, 3> &v2, double x, double y) {
            return ((v1[1]-v2[1])*x - (v1[0]-v2[0])*y + v1[0]*v2[1] - v2[0]*v1[1]);
        }

/**
 * Tests if pixel is inside a triangle
 *
 */
        bool inside_test(double A1, double A2, double A3, double sum) {
            return (A1/sum >= 0 && A1/sum <= 1) && (A2/sum >= 0 && A2/sum <= 1) && (A3/sum >= 0 && A3/sum <= 1);
        }

        void render(const std::array<double, 16> &cameraMatrix, const std::array<double, 16> &projectionMatrix,
                    const std::array<double, 16> &viewportMatrix) override {

            std::array<double, 3> proj_v1{};
            std::array<double, 3> proj_v2{};
            std::array<double, 3> proj_v3{};
            double w1, w2, w3;

            for (auto &v : mesh.indices) {
                auto v1 = mesh.vertices[v[0]];
                auto v2 = mesh.vertices[v[1]];
                auto v3 = mesh.vertices[v[2]];

                // Model -> World -> Camera View
                // Scale -> Rotate -> Translate
                auto ndc_v1 = v1.transform(world).transform(cameraMatrix);
                auto ndc_v2 = v2.transform(world).transform(cameraMatrix);
                auto ndc_v3 = v3.transform(world).transform(cameraMatrix);

                // V copy = v1.copy();
                // V::transform(world, copy);
                // V::transform(camera, copy);

                w1 = ndc_v1.project(projectionMatrix, proj_v1);
                w2 = ndc_v2.project(projectionMatrix, proj_v2);
                w3 = ndc_v3.project(projectionMatrix, proj_v3);

                viewportMapping(viewportMatrix, proj_v1);
                viewportMapping(viewportMatrix, proj_v2);
                viewportMapping(viewportMatrix, proj_v3);

                bounding_box box = compute_box(proj_v1, proj_v2, proj_v3);

                for (int r = std::lround(box.top); r < box.bottom; r++) {
                    for (int c = std::lround(box.left); c < box.right; c++) {
                        // Computes barycentric coordinates
                        double A1 = 0.5 * triangle_area(proj_v3, proj_v2, c, r);
                        double A2 = 0.5 * triangle_area(proj_v1, proj_v3, c, r);
                        double A3 = 0.5 * triangle_area(proj_v2, proj_v1, c, r);
                        double sum = A1 + A2 + A3;

                        if (inside_test(A1, A2, A3, sum)) {
                            int a = 1;
                            // ZBuffer Test ... Come ?
                        }
                    }
                }

                int a = 1;
            }
        }

        // ~Concrete_Object_Impl() override = default;
    };

}






#endif //INC_3D_RENDERER_CONCRETE_OBJECT_IMPL_H
