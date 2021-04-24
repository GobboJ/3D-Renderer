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

        void render(std::array<double, 16> &camera) override {
            for (auto &v : mesh.indices) {
                auto v1 = mesh.vertices[v[0]];
                auto v2 = mesh.vertices[v[1]];
                auto v3 = mesh.vertices[v[2]];

                // Model -> World -> Camera View
                // Scale -> Rotate -> Translate
                auto camera_v1 = v1.transform(world).transform(camera);
                auto camera_v2 = v2.transform(world).transform(camera);
                auto camera_v3 = v3.transform(world).transform(camera);
            }
        }

        // ~Concrete_Object_Impl() override = default;
    };

}

#endif //INC_3D_RENDERER_CONCRETE_OBJECT_IMPL_H
