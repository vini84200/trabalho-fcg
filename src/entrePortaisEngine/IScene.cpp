#include "entrePortaisEngine/IScene.hpp"

void entre_portais::IScene::renderImGui(bool *p_open) {

    if (ImGui::Begin(getName(), p_open)) {
        ImGui::Text("Scene");
        if (ImGui::CollapsingHeader("Objects")) {
            for (auto &child: children_) {
                child->renderImGui(nullptr);
            }
        }
        ImGui::End();
    } else {
        ImGui::End();
    }
}

void entre_portais::IScene::render() {
    renderer_->render(camera_.get());
}

const std::shared_ptr<entre_portais::Renderer> &entre_portais::IScene::getRenderer() const {
    return renderer_;
}

entre_portais::IScene::IScene(char *name) : IGameNode(name) {
    renderer_ = std::make_shared<Renderer>();
    physicsEngine_ = std::make_shared<PhysicsEngine>();
}

entre_portais::Window *entre_portais::IScene::getWindow() {
    return window_;
}

void entre_portais::IScene::setWindow(entre_portais::Window *window) {
    window_ = window;
}

const std::shared_ptr<entre_portais::PhysicsEngine> &entre_portais::IScene::getPhysicsEngine() const {
    return physicsEngine_;
}

void entre_portais::IScene::setPhysicsEngine(const std::shared_ptr<PhysicsEngine> &physicsEngine) {
    physicsEngine_ = physicsEngine;
}

void entre_portais::IScene::update(float deltaTime) {
    physicsEngine_->update(deltaTime);
}

void entre_portais::IScene::onResize(int width, int height) {
    renderer_->onWindowResize(width, height);
}

void entre_portais::IScene::renderImGuiWindows(bool *p_tree_open, bool *p_physics_open, bool *p_renderer_open) {
    renderImGui(p_tree_open);
    physicsEngine_->renderImGui(*camera_.get());
    renderer_->renderImGui();
}
