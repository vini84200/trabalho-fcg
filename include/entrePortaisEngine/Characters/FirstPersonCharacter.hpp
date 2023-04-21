#ifndef ENTREPORTAIS_FIRSTPERSONCHARACTER_HPP
#define ENTREPORTAIS_FIRSTPERSONCHARACTER_HPP

#include "characterkinematic/PxCapsuleController.h"
#include "characterkinematic/PxController.h"
#include "entrePortaisEngine/IObject.hpp"
#include "testGame/EmptyObject.hpp"

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

        void switchCameraMode();


    protected:
        glm::vec3 direction_ = {0, 0, 0};
        std::shared_ptr<Camera> camera_;
        std::shared_ptr<EmptyObject> emptyObject1_;
        std::shared_ptr<EmptyObject> emptyObject2_;
        bool sprint_;
        bool altMode_;
        bool pauseMode_;
        float speed_ = 2;
        physx::PxController *controller_;
        physx::PxU32 isOnGround_;
        float verticalAcceleration_;
    };

} // entre_portais

#endif //ENTREPORTAIS_FIRSTPERSONCHARACTER_HPP
