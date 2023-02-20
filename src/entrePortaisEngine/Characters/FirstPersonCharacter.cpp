#include "entrePortaisEngine/Characters/FirstPersonCharacter.hpp"
#include "GLFW/glfw3.h"
#include "entrePortaisEngine/IScene.hpp"

namespace entre_portais {
    FirstPersonCharacter::FirstPersonCharacter(char *name) : IObject(name) {

    }

    void FirstPersonCharacter::onKey(int key, int scancode, int action, int mods) {
        switch (key) {
            case GLFW_KEY_W:
                if (action == GLFW_PRESS) {
                    direction_ += glm::vec3(0, 0, -1);
                } else if (action == GLFW_RELEASE) {
                    direction_ -= glm::vec3(0, 0, -1);
                }
                break;
            case GLFW_KEY_A:
                if (action == GLFW_PRESS) {
                    direction_ += glm::vec3(-1, 0, 0);
                } else if (action == GLFW_RELEASE) {
                    direction_ -= glm::vec3(-1, 0, 0);
                }
                break;
            case GLFW_KEY_S:
                if (action == GLFW_PRESS) {
                    direction_ += glm::vec3(0, 0, 1);
                } else if (action == GLFW_RELEASE) {
                    direction_ -= glm::vec3(0, 0, 1);
                }
                break;
            case GLFW_KEY_D:
                if (action == GLFW_PRESS) {
                    direction_ += glm::vec3(1, 0, 0);
                } else if (action == GLFW_RELEASE) {
                    direction_ -= glm::vec3(1, 0, 0);
                }
                break;
        }
    }

    void FirstPersonCharacter::onMouseButton(int button, int action, int mods) {

    }

    void FirstPersonCharacter::onExit() {

    }

    void FirstPersonCharacter::update(float deltaTime) {
        auto newDirection = transform_.rotateVector(direction_);
        transform_.move(newDirection * deltaTime);
    }

    void FirstPersonCharacter::initialize() {
        loadBodyMesh();
        char *cameraName = "camera";
        auto camera = std::make_shared<Camera>(cameraName, M_PI_2, 0.1, 10000.0, 1.8);
        camera_ = camera;
        addChild(camera);
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("primitive");
        submit(renderer);
    }

    std::shared_ptr<Camera> FirstPersonCharacter::getCamera() {
        return camera_;
    }

    void FirstPersonCharacter::onResize(int width, int height) {

    }
} // entre_portais