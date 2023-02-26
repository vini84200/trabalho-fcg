#ifndef ENTREPORTAIS_BOXCOLLIDER_HPP
#define ENTREPORTAIS_BOXCOLLIDER_HPP

#include "ICollider.hpp"

namespace entre_portais {

    class BoxCollider : public ICollider {
    public:
        BoxCollider(glm::vec3 size, glm::mat4 &modelMatrix);

        ~BoxCollider() override = default;

        void Draw() override;

        void renderImGui() override;

        BoundingBox getBoundingBox(glm::mat4 transform) override;

        BoundingBox getBoundingBox() override;

        collisions::PossibleCollision isColliding(ICollider *other) override;

        collisions::PossibleCollision isColliding(glm::vec4 point) override;

        collisions::PossibleCollision isColliding(BoxCollider box) override;

        collisions::PossibleCollision isColliding(SphereCollider sphere) override;

    private:
        glm::vec3 size_;
    };

} // entre_portais

#endif //ENTREPORTAIS_BOXCOLLIDER_HPP
