//
// Created by Jonathan on 09/04/2021.
//
#include <tuple>
#include <memory>
#include <array>
#include "Object_Impl.h"
#include "Concrete_Object_Impl.h"

#ifndef INC_3D_RENDERER_OBJECT_H
#define INC_3D_RENDERER_OBJECT_H

using namespace Object_private;

class Object {

private:
    // (() -> void)[] funcs
    std::unique_ptr<Object_Impl> pimpl;

public:

    Object(const Object &) = delete;

    Object(Object &&) = default;

    template<class M, class S, class... T>
    Object(M &&mesh, S &&shader, const T &&... textures) :
            pimpl(std::make_unique<Concrete_Object_Impl<M, S>>(std::forward<M>(mesh), std::forward<S>(
                    shader), std::forward<T>(textures) ...)) {

        /*
         * Concrete_Object_Impl<M,S> objToCall = new Concrete_Object_Impl<M,S>(std::forward<M>(mesh), std::forward<S>(
                    shader), std::forward<T>(textures) ...)
         * this.funcs.push_back([objToCall]() -> objToCall.render())
         *
         */
    }

    void setPosition(const double x, const double y, const double z) {
        pimpl->setPosition(x, y, z);
    }

    void setRotation(const double rot_x, const double rot_y, const double rot_z) {
        pimpl->setRotation(rot_x, rot_y, rot_z);
    }

    void setScale(const double scale_x, const double scale_y, const double scale_z) {
        pimpl->setScale(scale_x, scale_y, scale_z);
    }

    void render(const std::array<double, 16> &cameraMatrix, const std::array<double, 16> projectionMatrix, const std::array<double, 16> &viewportMatrix) const {
        pimpl->render(cameraMatrix, projectionMatrix, viewportMatrix);
    }

    /*
     * render() {
     *  for (auto fun : this.funcs) {
     *      fun();
     * }
     */
};


#endif //INC_3D_RENDERER_OBJECT_H
