#include "labirinto/Player.hpp"
#include "entrePortaisEngine/IScene.hpp"
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
    
//        transform_.setPosition(glm::vec3(35.0f, 3.0f, 0.0f));
        teleport({35.0f, 3.0f, 0.0f});
    }
}

void labirinto::Player::initialize() {
    FirstPersonCharacter::initialize();
    // Physics
    auto physics = getScene()->getPhysicsEngine();
    // TODO: Implementar rigidbody
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
