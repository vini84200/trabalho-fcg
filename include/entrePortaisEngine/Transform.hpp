#ifndef ENTREPORTAIS_TRANSFORM_HPP
#define ENTREPORTAIS_TRANSFORM_HPP

#include <vector>
#include <functional>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace entre_portais {
    struct Transform {

        Transform();

        glm::mat4 getLocalModelMatrix() const;

        void setPosition(glm::vec3 position);

        void setScale(glm::vec3 scale);

        void setRotationZYX(glm::vec3 rotation);

        void setRotationZYX(float x, float y, float z);

        void setRotation(glm::quat rotation);

        void rotateBy(glm::vec3 rotation);

        void rotateBy(float x, float y, float z);

        void rotateBy(glm::quat rotation);

        glm::vec3 rotateVector(glm::vec3 vector) const;

        glm::vec4 getPosition() const;

        glm::quat getRotation() const;

        glm::vec3 getScale() const;

        glm::vec3 getForward() const;

        glm::vec3 getRotationEulerZYX() const;

        void renderImGui();

        void handleTransformChange() const;

        void onTransformChange(std::function<void()> callback);

        void move(glm::vec3 vector);

    private:
        glm::vec3 position_;
        glm::quat rotation_;
        glm::vec3 scale_;
        std::vector<std::function<void()>> on_transform_change_callbacks_;
    };
}
#endif //ENTREPORTAIS_TRANSFORM_HPP
