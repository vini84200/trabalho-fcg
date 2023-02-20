//
// Created by vini84200 on 2/19/23.
//

#include "testGame/CubeObject.hpp"
#include "entrePortaisEngine/meshes/CubeMesh.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/CylinderMesh.hpp"

namespace entre_portais {
    CubeObject::CubeObject(char *name) : IObject(name) {
        mesh_ = std::make_shared<CubeMesh>(1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        transform_.setPosition(glm::vec3(0, 0, 0));
        transform_.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        transform_.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    void CubeObject::onKey(int key, int scancode, int action, int mods) {

    }

    void CubeObject::onMouseButton(int button, int action, int mods) {

    }

    void CubeObject::onExit() {

    }

    void CubeObject::CustomImGui() {
    }

    void CubeObject::update(float deltaTime) {
    }

    void CubeObject::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("primitive");
        submit(renderer);

    }

    void CubeObject::onResize(int width, int height) {
    }
} // entre_portais