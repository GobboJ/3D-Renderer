//
// Created by Jonathan on 09/04/2021.
//

#ifndef INC_3D_RENDERER_CONCRETE_OBJECT_IMPL_H
#define INC_3D_RENDERER_CONCRETE_OBJECT_IMPL_H

namespace Object_private {

    template<class M, class S, class ...T>
    class Concrete_Object_Impl : public Object_Impl {

    private:
        M mesh;
        S shader;
        std::tuple<T...> textures;

    public:
        //
        Concrete_Object_Impl(M &&mesh, S &&shader) : mesh(std::forward<M>(mesh)),
                                                     shader(std::forward<S>(shader)),
                                                     textures(std::forward<T>(textures) ...) {}

        // ~Concrete_Object_Impl() override = default;
    };

}

#endif //INC_3D_RENDERER_CONCRETE_OBJECT_IMPL_H
