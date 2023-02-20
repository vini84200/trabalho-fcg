//
// Created by vini84200 on 2/19/23.
//

#ifndef ENTREPORTAIS_CYLINDERMESH_HPP
#define ENTREPORTAIS_CYLINDERMESH_HPP

#include "IMesh.hpp"

namespace entre_portais {

    class CylinderMesh : public IMesh {
    public:
        CylinderMesh(float radius, float height, int edges, glm::vec3 color);

        void Draw() override;
    };

} // entre_portais

#endif //ENTREPORTAIS_CYLINDERMESH_HPP
