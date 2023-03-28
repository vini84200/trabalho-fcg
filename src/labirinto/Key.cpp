#include "labirinto/Key.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "labirinto/InvisibleBox.hpp"
#include "GLFW/glfw3.h"

namespace labirinto {

    glm::vec2 Key::getPlayerPosition() {
        // Retorna a posição do jogador
        auto playerPosition = getParent()->getScene()->getCamera()->getParent()->getParent()->getParent()->getTransform()->getPosition();
        return glm::vec2(playerPosition.x, playerPosition.z);
    }

    void Key::onKey(int key, int scancode, int action, int mods) {
        if ((key == GLFW_KEY_SPACE || key == GLFW_KEY_E ) && action == GLFW_PRESS) {
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

            if (isDoor_) {
                spdlog::info("You won");
            } else {
                spdlog::info("You lost");
            }
        }
    }

    void Key::onMouseButton(int button, int action, int mods) {

    }

    void Key::onExit() {

    }

    Key::Key(bool isDoor, char *name) : IObject(name), isDoor_(isDoor) {

    }


    void Key::update(float deltaTime) {
    }

    void Key::initialize() {
        // mesh_ = std::make_shared<entre_portais::MeshFromObj>(path);
        if (mesh_.get() == nullptr) {
            spdlog::error("Mesh is null at Key, should have been set before calling initialize");
        }
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("phong");
        submit(renderer, entre_portais::RenderPass::FOREGROUND);
    }

    void Key::onResize(int width, int height) {
    }
} // labirinto