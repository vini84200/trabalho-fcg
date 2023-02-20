#include "testGame/CylinderObject.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/CylinderMesh.hpp"

namespace entre_portais {
    void CylinderObject::CustomImGui() {

    }

    void CylinderObject::update(float deltaTime) {
    }

    void CylinderObject::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("primitive");
        submit(renderer);
    }

    void CylinderObject::onResize(int width, int height) {
    }

    void CylinderObject::onKey(int key, int scancode, int action, int mods) {
    }

    void CylinderObject::onMouseButton(int button, int action, int mods) {
    }

    void CylinderObject::onExit() {

    }

    CylinderObject::CylinderObject(char *name) : IObject(name) {
        mesh_ = std::make_shared<CylinderMesh>(0.3f, 1.0f, 10000, glm::vec3(0.0f, 1.0f, 0.0f));
        transform_.setPosition(glm::vec3(-1, 0, 0));
        transform_.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        transform_.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    }
} // entre_portais