//
// Created by vini84200 on 2/19/23.
//

#ifndef ENTREPORTAIS_CUBEMESH_HPP
#define ENTREPORTAIS_CUBEMESH_HPP

#include "IMesh.hpp"

namespace entre_portais {

    class CubeMesh : public IMesh {
    public:
        CubeMesh(float size, glm::vec3 color);

        void Draw(Shader shaderInUse) override;


    };

} // entre_portais

#endif //ENTREPORTAIS_CUBEMESH_HPP
