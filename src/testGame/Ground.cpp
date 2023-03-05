#include "testGame/Ground.hpp"
#include "entrePortaisEngine/meshes/PlaneMesh.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/physics/BoxCollider.hpp"

namespace entre_portais {

    void Ground::onKey(int key, int scancode, int action, int mods) {

    }

    void Ground::onMouseButton(int button, int action, int mods) {

    }

    void Ground::onExit() {

    }

    Ground::Ground() : IObject("Ground") {
        loadShader("primitive");
        mesh_ = std::make_shared<PlaneMesh>(10.0f, 10.0f, 20, 20, PLANE_COLORIDO);
        transform_.setPosition(glm::vec3(0, 0, 0));
        transform_.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        transform_.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    void Ground::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        submit(renderer);
        std::unique_ptr<ICollider> cubeCollider = std::make_unique<BoxCollider>(glm::vec3(10.0f, 0.1f, 10.0f),
                                                                                modelMatrix_);
        rigidBody_ = std::make_unique<RigidBody>(&modelMatrix_, std::move(cubeCollider),
                                                 *this->getScene()->getPhysicsEngine().get(),
                                                 this->transform_);
    }

    void Ground::update(float deltaTime) {
    }

    void Ground::onResize(int width, int height) {
    }
} // entre_portais