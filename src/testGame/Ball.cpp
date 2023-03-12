#include "testGame/Ball.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "entrePortaisEngine/physics/SphereCollider.hpp"

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
        submit(renderer);

        auto sphereCollider = std::make_unique<SphereCollider>(1, modelMatrix_);
        rigidBody_ = std::make_unique<RigidBody>(&modelMatrix_, std::move(sphereCollider),
                                                 *this->getScene()->getPhysicsEngine().get(),
                                                 this->transform_);

        rigidBody_->setInertiaTensor(matrices::inertiaTensorSphere(1, 1));
//        rigidBody_->setIsStatic(false);
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