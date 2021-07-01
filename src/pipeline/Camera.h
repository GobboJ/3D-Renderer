//
// Created by Jonathan on 09/04/2021.
//
#include <array>
#include "util/Vector3.h"

#ifndef INC_3D_RENDERER_CAMERA_H
#define INC_3D_RENDERER_CAMERA_H

/**
 * Represents a Camera in the scene
 */
class Camera {

private:

    // View Matrix (World -> Camera coordinates)
    std::array<double, 16> viewMatrix{};
    // Vertical field of view
    double verticalFieldOfView;
    // Near plane
    double nearPlane;
    // Far plane
    double farPlane;

public:

    /**
     * Creates a new look-at camera.
     *
     * @param verticalFieldOfView Vertical FOV of the camera
     * @param nearPlane Near projection plane. It must be positive.
     * @param farPlane Far projection plane. It must be positive.
     * @param position Position of the camera.
     * @param target Point the camera will look at.
     * @param up_vector Vector describing the up direction. By default it is the vector <code>[0, 1, 0]</code>.
     */
    Camera(double verticalFieldOfView, double nearPlane, double farPlane, const Vector3 &position,
           const Vector3 &target,
           const Vector3 &up_vector = {0, 1, 0});

    /**
     * Creates a new FPS camera. NOT IMPLEMENTED
     *
     * @param verticalFieldOfView Vertical FOV of the camera
     * @param nearPlane Near projection plane
     * @param farPlane Far projection plane
     * @param position Position of the camera.
     * @param pitch Pitch (rotation around x axis)
     * @param yaw Yaw (rotation around y axis)
     */
    Camera(double verticalFieldOfView, double nearPlane, double farPlane, const Vector3 &position, double pitch,
           double yaw);

    /**
     * Returns the generated view matrix
     * @return the generated view matrix
     */
    const std::array<double, 16> &getViewMatrix() const;

    /**
     * Returns the near plane
     */
    double getNearPlane() const;

    /**
     * Returns the far plane
     */
    double getFarPlane() const;

    /**
     * Returns the vertical field of view
     */
    double getVerticalFieldOfView() const;

};

#endif //INC_3D_RENDERER_CAMERA_H
