#include "labirinto/Player.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/physics/BoxCollider.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"

void labirinto::Player::loadBodyMesh() {
    mesh_ = std::make_shared<entre_portais::MeshFromObj>("assets/objs/leoncio.obj");
}

void labirinto::Player::onKey(int key, int scancode, int action, int mods) {
    FirstPersonCharacter::onKey(key, scancode, action, mods);
}

void labirinto::Player::onMouseButton(int button, int action, int mods) {
    FirstPersonCharacter::onMouseButton(button, action, mods);
}

void labirinto::Player::onExit() {
    FirstPersonCharacter::onExit();
}

void labirinto::Player::update(float deltaTime) {
    FirstPersonCharacter::update(deltaTime);

    if (transform_.getPosition().y < -1.f) {
       // TP player 
    
        transform_.setPosition(glm::vec3(0,0,0));
        rigidBody_->setVelocity(glm::vec3(0,0,0));
    }
}

void labirinto::Player::initialize() {
    FirstPersonCharacter::initialize();
    // Physics
    auto physics = getScene()->getPhysicsEngine();
    auto collider = std::make_unique<entre_portais::BoxCollider>(glm::vec3(0.5f, 3.f, 0.5f), modelMatrix_);
    rigidBody_ = std::make_unique<entre_portais::RigidBody>(&modelMatrix_, std::move(collider), *physics.get(),
                                                            transform_);
    rigidBody_->setInertiaTensor(matrices::inertiaTensorBox(60, glm::vec3(0.5f, 1.8f, 0.5f)));
    rigidBody_->setMass(60);
    rigidBody_->setIsStatic(false);

    // Move the camera to the player eyes
    emptyObject1_->getTransform()->setPosition(glm::vec3(0, 0.8f, 0));
}

void labirinto::Player::onResize(int width, int height) {
    FirstPersonCharacter::onResize(width, height);
}

void labirinto::Player::render(entre_portais::RenderPass current_pass) {
    if (pauseMode_)
        IObject::render(current_pass);
}

labirinto::Player::Player(char *name) : FirstPersonCharacter(name) {

}
