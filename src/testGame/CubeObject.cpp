#include "testGame/CubeObject.hpp"
#include "entrePortaisEngine/meshes/CubeMesh.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/CylinderMesh.hpp"
#include "entrePortaisEngine/physics/RigidBody.hpp"
#include "entrePortaisEngine/physics/BoxCollider.hpp"

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
        std::unique_ptr<ICollider> cubeCollider = std::make_unique<BoxCollider>(glm::vec3(1.0f, 1.0f, 1.0f),
                                                                                modelMatrix_);
        rigidBody_ = std::make_unique<RigidBody>(&modelMatrix_, std::move(cubeCollider),
                                                 *this->getScene()->getPhysicsEngine().get(),
                                                 this->transform_);
        rigidBody_->setInertiaTensor(matrices::inertiaTensorBox(1, glm::vec3(1.0f, 1.0f, 1.0f)));
        rigidBody_->setIsStatic(false);
        loadShader("primitive");
        submit(renderer, RenderPass::FOREGROUND);

    }

    void CubeObject::onResize(int width, int height) {
    }
} // entre_portais
