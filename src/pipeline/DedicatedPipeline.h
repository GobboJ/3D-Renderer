//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_DEDICATEDPIPELINE_H
#define INC_3D_RENDERER_DEDICATEDPIPELINE_H

#include <array>

template <class target_t>
class DedicatedPipeline {
public:
    virtual void render(target_t* target, double* z_buffer, unsigned int width, unsigned int height, const std::array<double, 16> &viewMatrix,
                        const std::array<double, 16> &projectionMatrix, const std::array<double, 16> &viewportMatrix) = 0;
};


#endif //INC_3D_RENDERER_DEDICATEDPIPELINE_H
