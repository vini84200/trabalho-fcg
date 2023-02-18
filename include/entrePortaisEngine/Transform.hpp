#ifndef ENTREPORTAIS_TRANSFORM_HPP
#define ENTREPORTAIS_TRANSFORM_HPP

#include "glm/glm.hpp"

namespace entre_portais {
    struct Transform {
        float x, y, z;
        float rx, ry, rz;
        float sx, sy, sz;

        Transform();

        glm::mat4 getModelMatrix();

        void setPosition(glm::vec3 position);

        void setScale(glm::vec3 scale);

        void setRotation(glm::vec3 rotation);

        float *getPositionPtr();

        float *getRotationPtr();

        float *getScalePtr();

        glm::vec4 getPosition();
    };
}
#endif //ENTREPORTAIS_TRANSFORM_HPP
