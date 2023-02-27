#ifndef ENTREPORTAIS_FIRSTPERSONCHARACTER_HPP
#define ENTREPORTAIS_FIRSTPERSONCHARACTER_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {

    class FirstPersonCharacter : public IObject {
    public:
        FirstPersonCharacter(char *name);

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        void update(float deltaTime) override;

        void initialize() override;

        virtual void loadBodyMesh() = 0;

        void onResize(int width, int height) override;

        std::shared_ptr<Camera> getCamera();

        void onMouseDeltaMovement(glm::vec2 delta) override;


    private:
        glm::vec3 direction_ = {0, 0, 0};
        std::shared_ptr<Camera> camera_;
        bool altMode_;
    };

} // entre_portais

#endif //ENTREPORTAIS_FIRSTPERSONCHARACTER_HPP
