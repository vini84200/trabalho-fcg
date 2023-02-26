#ifndef ENTREPORTAIS_BOXCOLLIDER_HPP
#define ENTREPORTAIS_BOXCOLLIDER_HPP

#include "ICollider.hpp"

namespace entre_portais {

    class BoxCollider : public ICollider {
    public:
        BoxCollider(glm::vec3 size);

        ~BoxCollider() override = default;

        void Draw() override;

        void renderImGui() override;

        BoundingBox getBoundingBox(glm::mat4 transform) override;

        BoundingBox getBoundingBox() override;

    private:
        glm::vec3 size_;
    };

} // entre_portais

#endif //ENTREPORTAIS_BOXCOLLIDER_HPP
