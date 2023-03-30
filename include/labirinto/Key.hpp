#ifndef ENTREPORTAIS_Key_HPP
#define ENTREPORTAIS_Key_HPP

#include "entrePortaisEngine/IObject.hpp"
#include "entrePortaisEngine/Bezier.hpp"

namespace labirinto {

    class Key : public entre_portais::IObject {
    public:
        Key(bool isDoor, char *name);

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        glm::vec2 getPlayerPosition();

    private:
        bool isDoor_;
        bool animationRunning_ = false;
        float t = 0.f;
        entre_portais::Bezier animation;
    };

} // labirinto

#endif //ENTREPORTAIS_Key_HPP
