//
// Created by Jonathan on 06/05/2021.
//

#ifndef INC_3D_RENDERER_DEDICATEDPIPELINE_H
#define INC_3D_RENDERER_DEDICATEDPIPELINE_H

#include <array>

/**
 * Describes the base interface of the dedicated rendering pipeline
 * @tparam target_t Type of the target
 */
template <class target_t>
class DedicatedPipeline {

public:

    /**
     * Renders on the target
     * @param target The target
     * @param z_buffer The z-buffer of the target
     * @param width Width of the target
     * @param height Height of the target
     * @param viewMatrix The view matrix
     * @param projectionMatrix The projection matrix
     * @param viewportMatrix The viewport matrix
     */
    virtual void render(target_t* target, double* z_buffer, unsigned int width, unsigned int height, const std::array<double, 16> &viewMatrix,
                        const std::array<double, 16> &projectionMatrix, const std::array<double, 16> &viewportMatrix) = 0;

};

#endif //INC_3D_RENDERER_DEDICATEDPIPELINE_H
