#include "entrePortaisEngine/physics/BoxCollider.hpp"

namespace entre_portais {
    void BoxCollider::Draw() {

    }

    void BoxCollider::renderImGui() {

    }

    BoundingBox BoxCollider::getBoundingBox(glm::mat4 transform) {
        // Cantos do cubo
        auto bbo = getBoundingBox();
        glm::vec3 points[8] = {
                bbo.min,
                {bbo.min.x, bbo.min.y, bbo.max.z},
                {bbo.min.x, bbo.max.y, bbo.min.z},
                {bbo.min.x, bbo.max.y, bbo.max.z},
                {bbo.max.x, bbo.min.y, bbo.min.z},
                {bbo.max.x, bbo.min.y, bbo.max.z},
                {bbo.max.x, bbo.max.y, bbo.min.z},
                bbo.max
        };

        // Transforma os cantos
        auto new_bbo = BoundingBox(transform * glm::vec4(points[0], 1.0f));
        for (int i = 1; i < 8; i++) {
            auto point = glm::vec3(transform * glm::vec4(points[i], 1.0f));
            new_bbo.addPoint(point);
        }

        return new_bbo;
    }

    BoxCollider::BoxCollider(glm::vec3 size) {
        size_ = size;
    }

    BoundingBox BoxCollider::getBoundingBox() {
        return {glm::vec3(-size_ / 2.0f), glm::vec3(size_ / 2.0f)};
    }

} // entre_portais