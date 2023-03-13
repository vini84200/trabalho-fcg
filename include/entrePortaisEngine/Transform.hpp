#ifndef ENTREPORTAIS_TRANSFORM_HPP
#define ENTREPORTAIS_TRANSFORM_HPP

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace entre_portais {
    struct Transform {

        Transform();

        glm::mat4 getModelMatrix();

        void setPosition(glm::vec3 position);

        void setScale(glm::vec3 scale);

        void setRotation(glm::vec3 rotation);

        void setRotation(float x, float y, float z);

        void setRotation(glm::quat rotation);

        void rotateBy(glm::vec3 rotation);

        void rotateBy(float x, float y, float z);

        void rotateBy(glm::quat rotation);

        glm::vec3 rotateVector(glm::vec3 vector);

        float *getPositionPtr();

        float *getRotationPtr();

        float *getScalePtr();

        glm::vec4 getPosition();

        glm::quat getRotation();

        glm::vec3 getScale();

        glm::vec3 getForward();

        glm::vec3 getRotationEuler();

        void renderImGui();

        void move(glm::vec3 vector);

    private:
        glm::vec3 position_;
        glm::quat rotation_;
        glm::vec3 scale_;
        bool lock_rotation_;
    };
}
#endif //ENTREPORTAIS_TRANSFORM_HPP
