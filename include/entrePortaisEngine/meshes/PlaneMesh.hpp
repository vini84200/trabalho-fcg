//
// Created by vini84200 on 2/19/23.
//

#ifndef ENTREPORTAIS_PLANEMESH_HPP
#define ENTREPORTAIS_PLANEMESH_HPP

#include "IMesh.hpp"

namespace entre_portais {

    class PlaneMesh : public IMesh {
    public:
        PlaneMesh(float width, float height, int widthSegments, int heightSegments);

        PlaneMesh(float width, float height, int segments); // widthSegments = heightSegments = segments
        PlaneMesh(float width, float height); // segments = 1
        void Draw() override;

    private:
        float width_;
        float height_;
        int widthSegments_;
        int heightSegments_;
    };

} // entre_portais

#endif //ENTREPORTAIS_PLANEMESH_HPP
