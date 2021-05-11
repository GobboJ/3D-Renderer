//
// Created by Jonathan on 10/05/2021.
//

#ifndef INC_3D_RENDERER_TEXTUREVERTEX_H
#define INC_3D_RENDERER_TEXTUREVERTEX_H


#include <array>

class TextureVertex {
private:
    double x;
    double y;
    double z;
    double u;
    double v;

public:
    TextureVertex(double x, double y, double z, double u, double v);

    double getX() const;

    double getY() const;

    double getZ() const;

    double getU() const;

    double getV() const;

    void transform(const std::array<double, 16> &matrix);

    double project(const std::array<double, 16> &projectionMatrix);

    void viewportMapping(const std::array<double, 16> &viewportMatrix);

    static TextureVertex
    interpolate(const TextureVertex &v1, const TextureVertex &v2, const TextureVertex &v3, double A1,
                double A2, double A3, double w0, double w1, double w2);

private:
    inline double multiply(const std::array<double, 16> &matrix, int index) const;
};


#endif //INC_3D_RENDERER_TEXTUREVERTEX_H
