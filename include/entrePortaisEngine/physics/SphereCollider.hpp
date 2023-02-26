#ifndef ENTREPORTAIS_SPHERECOLLIDER_HPP
#define ENTREPORTAIS_SPHERECOLLIDER_HPP

#include "ICollider.hpp"

namespace entre_portais {

    class SphereCollider : public ICollider {
    public:
        SphereCollider(glm::mat4 &modelMatrix);

        SphereCollider(float radius, glm::mat4 &modelMatrix);

        ~SphereCollider() override = default;

        collisions::PossibleCollision isColliding(ICollider *other) override;

        collisions::PossibleCollision isColliding(glm::vec4 point) override;

        collisions::PossibleCollision isColliding(BoxCollider box) override;

        collisions::PossibleCollision isColliding(SphereCollider sphere) override;

        void Draw() override;

        void renderImGui() override;

        BoundingBox getBoundingBox(glm::mat4 modelMatrix) override;

        BoundingBox getBoundingBox() override;

    private:
        float radius_;
    };

} // entre_portais

#endif //ENTREPORTAIS_SPHERECOLLIDER_HPP
