#include "testGame/Ball.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "geometry/PxSphereGeometry.h"

namespace entre_portais {
    Ball::Ball(const char *name) : IObject(name) {
    }


    void Ball::update(float deltaTime) {
    }

    void Ball::initialize() {
        mesh_ = std::make_shared<MeshFromObj>("assets/objs/sphere.obj");
        spdlog::info("Initializing Ball");
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("phong");
        submit(renderer, RenderPass::FOREGROUND);


        physx::PxSphereGeometry geometry = physx::PxSphereGeometry(1.f);
        physx::PxMaterial *material = getScene()->getPhysicsEngine()->getPhysics().createMaterial(0.5f, 0.5f, 0.6f);
        rigidBody_ = std::make_unique<PhysicsActor>(*getScene()->getPhysicsEngine().get(),
                                                    transform_,
                                                    geometry,
                                                    *material,
                                                    1.f, true);
    }

    void Ball::onKey(int key, int scancode, int action, int mods) {

    }

    void Ball::onMouseButton(int button, int action, int mods) {

    }

    void Ball::onExit() {

    }

    void Ball::onResize(int width, int height) {
    }

} // entre_portais