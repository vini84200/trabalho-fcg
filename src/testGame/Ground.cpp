#include "testGame/Ground.hpp"
#include "PxMaterial.h"
#include "entrePortaisEngine/meshes/PlaneMesh.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/physics/PhysicsScene.hpp"
#include "entrePortaisEngine/physics/PhysicsActor.hpp"
#include "extensions/PxSimpleFactory.h"
#include "geometry/PxBoxGeometry.h"
#include "geometry/PxGeometry.h"
#include "geometry/PxPlaneGeometry.h"

namespace entre_portais {

    void Ground::onKey(int key, int scancode, int action, int mods) {

    }

    void Ground::onMouseButton(int button, int action, int mods) {

    }

    void Ground::onExit() {

    }

    Ground::Ground() : IObject("Ground") {
        loadShader("primitive");
        mesh_ = std::make_shared<PlaneMesh>(10.0f, 10.0f, 20, 20, PLANE_CINZA);
        transform_.setPosition(glm::vec3(0, 0, 0));
        transform_.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        transform_.setRotation(glm::quat(0.7071f, 0.0f, 0.0f, 0.7071f));
    }

    void Ground::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        submit(renderer, RenderPass::FOREGROUND);
        // TODO: Adicionar rigidbody
        physx::PxPlaneGeometry geometry = physx::PxPlaneGeometry();
        physx::PxMaterial *material = getScene()->getPhysicsEngine()->getPhysics().createMaterial(0.5f, 0.5f, 0.6f);
        rigidBody_ = std::make_unique<PhysicsActor>(*getScene()->getPhysicsEngine().get(),
                                                    transform_,
                                                    geometry,
                                                    *material,
                                                    1.f, false);

    }

    void Ground::update(float deltaTime) {
    }

    void Ground::onResize(int width, int height) {
    }
} // entre_portais