#include "labirinto/InvisibleBox.hpp"
#include "entrePortaisEngine/physics/BoxCollider.hpp"
#include "entrePortaisEngine/IScene.hpp"

InvisibleBox::InvisibleBox(int x, int y) : InvisibleBox(glm::vec3(x, 0, y), glm::vec3(2, 12, 2), "InvisibleBox") {

    // Generate the name from x, y
    char name[100];
    sprintf(name, "InvisibleBox_%d_%d", x, y);
    setName(name);
}

void InvisibleBox::initialize() {
    auto cubeCollider = std::make_unique<entre_portais::BoxCollider>(glm::vec3(1.0f, 1.0f, 1.0f),
                                                                     modelMatrix_);
    rigidBody_ = std::make_unique<entre_portais::RigidBody>(&modelMatrix_, std::move(cubeCollider),
                                                            *this->getScene()->getPhysicsEngine().get(),
                                                            this->transform_);
    rigidBody_->setInertiaTensor(matrices::inertiaTensorBox(1, glm::vec3(1.0f, 1.0f, 1.0f)));
    spdlog::info("InvisibleBox initialized");
}

void InvisibleBox::render(entre_portais::RenderPass current_pass) {
    // Invisble so nothing to render
}

void InvisibleBox::update(float deltaTime) {
}

void InvisibleBox::onResize(int width, int height) {
}

void InvisibleBox::onKey(int key, int scancode, int action, int mods) {
}

void InvisibleBox::onMouseButton(int button, int action, int mods) {
}

InvisibleBox::InvisibleBox(glm::vec3 pos, glm::vec3 scale, const char *name) : IObject(name) {
    transform_.setPosition(pos);
    transform_.setScale(scale);

}

void InvisibleBox::onExit() {

}
