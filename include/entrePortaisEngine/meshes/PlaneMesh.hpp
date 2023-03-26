//
// Created by vini84200 on 2/19/23.
//

#ifndef ENTREPORTAIS_PLANEMESH_HPP
#define ENTREPORTAIS_PLANEMESH_HPP

#include "IMesh.hpp"

namespace entre_portais {

    typedef unsigned int PlaneColor;
#define PLANE_COLORIDO (entre_portais::PlaneColor) 1
#define PLANE_BRANCO (entre_portais::PlaneColor) 2
#define PLANE_CINZA (entre_portais::PlaneColor) 3

    class PlaneMesh : public IMesh {
    public:

        PlaneMesh(float width, float height, int widthSegments, int heightSegments, PlaneColor cor);

        PlaneMesh(float width, float height, int widthSegments, int heightSegments);

        PlaneMesh(float width, float height, int segments); // widthSegments = heightSegments = segments
        PlaneMesh(float width, float height); // segments = 1
        void Draw(Shader shaderInUse, RenderPass current_pass) override;

    private:
        float width_;
        float height_;
        int widthSegments_;
        int heightSegments_;
    };

} // entre_portais

#endif //ENTREPORTAIS_PLANEMESH_HPP
