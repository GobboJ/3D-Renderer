//
// Created by Jonathan on 09/04/2021.
//
#include <tuple>
#include <memory>
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
    Object(M &&mesh, S &&shader, T &&... textures) :
            pimpl(std::make_unique<Concrete_Object_Impl<M, S>>(std::forward<M>(mesh), std::forward<S>(
                    shader), std::forward<T>(textures) ...)) {

        /*
         * Concrete_Object_Impl<M,S> objToCall = new Concrete_Object_Impl<M,S>(std::forward<M>(mesh), std::forward<S>(
                    shader), std::forward<T>(textures) ...)
         * this.funcs.push_back([objToCall]() -> objToCall.render())
         *
         */
    }

    /*
     * render() {
     *  for (auto fun : this.funcs) {
     *      fun();
     * }
     */
};


#endif //INC_3D_RENDERER_OBJECT_H
