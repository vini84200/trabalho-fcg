#include "labirinto/Key.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "labirinto/InvisibleBox.hpp"
#include "GLFW/glfw3.h"
#include "entrePortaisEngine/Window.hpp"
#include "labirinto/WinScene.hpp"
#include "labirinto/LoseScene.hpp"

namespace labirinto {

    glm::vec2 Key::getPlayerPosition() {
        // Retorna a posição do jogador
        auto playerPosition = getParent()->getScene()->getCamera()->getParent()->getParent()->getParent()->getTransform()->getPosition();
        return glm::vec2(playerPosition.x, playerPosition.z);
    }

    void Key::onKey(int key, int scancode, int action, int mods) {
        if ((key == GLFW_KEY_SPACE || key == GLFW_KEY_E) && action == GLFW_PRESS) {
            spdlog::info("Key pressed");
            // Calculate player position
            auto playerPosition = getPlayerPosition();
            // Calculate key position
            auto keyPosition = getTransform()->getPosition(); // Pode bugar se o obj não for raiz

            // Check if player is close enough to the key
            if (glm::distance(playerPosition, glm::vec2(keyPosition.x, keyPosition.z)) > 0.8f) {
                spdlog::info("Too far away");
                return;
            }

            if (!isDoor_) {
                spdlog::info("Player has lost");
                entre_portais::IObject::getScene()->getWindow()->setScene<LoseScene>();
                entre_portais::IObject::getScene()->getWindow()->showCursor(true);
            }
            animationRunning_ = true;
        }
    }

    void Key::onMouseButton(int button, int action, int mods) {

    }

    void Key::onExit() {

    }

    Key::Key(bool isDoor, char *name) : IObject(name), isDoor_(isDoor) {
    }


    void Key::update(float deltaTime) {
        if (animationRunning_) {
            t += deltaTime;
            float animationTime = t; // Permite depois mudar a função de interpolação 
            if (animationTime >= 1) {
                animationTime = 0.999f;
                if (t >= 2) {
                    // Player won
                    if (isDoor_) {
                        spdlog::info("Player won");
                        entre_portais::IObject::getScene()->getWindow()->setScene<WinScene>();
                        entre_portais::IObject::getScene()->getWindow()->showCursor(true);

                    } else {
                    }
                }
            }
            transform_.setPosition(animation.cubicPositionBezierCurve(animationTime));
            transform_.setRotation(animation.cubicRotationBezierCurve(animationTime));
        }
    }

    void Key::initialize() {
        // mesh_ = std::make_shared<entre_portais::MeshFromObj>(path);
        if (mesh_.get() == nullptr) {
            spdlog::error("Mesh is null at Key, should have been set before calling initialize");
        }
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("phong");
        submit(renderer, entre_portais::RenderPass::FOREGROUND);

        if (isDoor_) {
            animation = entre_portais::Bezier();
            animation.add(
                    transform_.getPosition(),
                    glm::quat(1, 0, 0, 0)
            );
            animation.add(
                    glm::vec3(transform_.getPosition()) + glm::vec3(0, 0.4, 0),
                    glm::quat(0.7, 0, 0.4, 0)
            );
            animation.add(
                    glm::vec3(transform_.getPosition()) + glm::vec3(0, 0.5, -.1),
                    glm::quat(0.707, 0, 0, 0.707)
            );
            animation.add(
                    glm::vec3(transform_.getPosition()) + glm::vec3(0, 0.5, -.2),
                    glm::quat(0.707, 0, 0, 0.707)
            );
        }
    }

    void Key::onResize(int width, int height) {
    }
} // labirinto