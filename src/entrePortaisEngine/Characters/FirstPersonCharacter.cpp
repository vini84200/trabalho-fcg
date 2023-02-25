#include "entrePortaisEngine/Characters/FirstPersonCharacter.hpp"
#include "GLFW/glfw3.h"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/Window.hpp"
#include "glm/gtx/string_cast.hpp"

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
            case GLFW_KEY_LEFT_ALT:
                if (action == GLFW_PRESS) {
                    IObject::getScene()->getWindow()->showCursor(true);
                    altMode_ = true;
                } else if (action == GLFW_RELEASE) {
                    IObject::getScene()->getWindow()->showCursor(false);
                    altMode_ = false;
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

    void FirstPersonCharacter::onMouseDeltaMovement(glm::vec2 delta) {
        if (altMode_) {
            return;
        }
        glm::vec2 d = 0.001f * delta;
        glm::vec3 rightCamera = glm::cross(glm::vec3(0.0, 1.0, 0.0), camera_->getTransform()->getForward());
        glm::quat rotationHead = glm::quat(glm::vec3(0, 0, -d.y));
        glm::quat rotationBody = glm::quat(glm::vec3(0, -d.x, 0));
        camera_->getTransform()->rotateBy(rotationHead);
        glm::vec3 newRightCamera = glm::cross(glm::vec3(0.0, 1.0, 0.0), camera_->getTransform()->getForward());
        if (glm::dot(rightCamera, newRightCamera) < 0) {
            camera_->getTransform()->rotateBy(conjugate(rotationHead));
        }
        getTransform()->rotateBy(rotationBody);
    }
} // entre_portais