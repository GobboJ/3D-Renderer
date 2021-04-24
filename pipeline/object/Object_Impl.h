//
// Created by Jonathan on 09/04/2021.
//

#ifndef INC_3D_RENDERER_OBJECT_IMPL_H
#define INC_3D_RENDERER_OBJECT_IMPL_H

namespace Object_private {
    class Object_Impl {

    public:
        virtual ~Object_Impl() = default;

        virtual void setPosition(double x, double y, double z) = 0;

        virtual void setRotation(double rot_x, double rot_y, double rot_z) = 0;

        virtual void setScale(double scale_x, double scale_y, double scale_z) = 0;

        virtual void render(std::array<double, 16> &camera) = 0;
    };
}

#endif //INC_3D_RENDERER_OBJECT_IMPL_H
