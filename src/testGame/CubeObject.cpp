#include "testGame/CubeObject.hpp"
#include "entrePortaisEngine/meshes/CubeMesh.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "geometry/PxBoxGeometry.h"

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
        submit(renderer, RenderPass::FOREGROUND);

        physx::PxBoxGeometry geometry = physx::PxBoxGeometry(0.5f, 0.5f, 0.5f);
        physx::PxMaterial *material = getScene()->getPhysicsEngine()->getPhysics().createMaterial(0.5f, 0.5f, 0.6f);
        rigidBody_ = std::make_unique<PhysicsActor>(*getScene()->getPhysicsEngine().get(),
                                                    transform_,
                                                    geometry,
                                                    *material,
                                                    1.f, true);
    }

    void CubeObject::onResize(int width, int height) {
    }
} // entre_portais
