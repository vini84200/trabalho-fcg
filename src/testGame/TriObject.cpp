#include "testGame/TriObject.hpp"
#include "entrePortaisEngine/EasyMesh.hpp"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "testGame/WaitAndFinishTask.hpp"
#include "entrePortaisEngine/tasks/TaskHandler.hpp"

entre_portais::TriObject::TriObject(char *name) : IObject(name), logger_(name) {
    auto vert = new entre_portais::ManyVertices();
    vert->vertices.push_back({-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f});
    vert->vertices.push_back({0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f});
    vert->vertices.push_back({0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f});
    vert->indices.push_back(0);
    vert->indices.push_back(1);
    vert->indices.push_back(2);
    mesh_ = std::make_shared<EasyMesh>(*vert, "assets/shaders/tri.vert", "assets/shaders/tri.frag");

    delete vert;
    transform_.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    transform_.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    transform_.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
}

void entre_portais::TriObject::update(double deltaTime) {
    // Gira o triângulo
    transform_.setRotation(
            glm::vec3(transform_.rx, transform_.ry, transform_.rz + velocity_ * (static_cast<float>(deltaTime))));

    if (transform_.rz > 360.0f) {
        transform_.rz -= 360.0f;
    }
}

void entre_portais::TriObject::initialize() {

}

void entre_portais::TriObject::onResize(int width, int height) {

}

void entre_portais::TriObject::onExit() {

}

void entre_portais::TriObject::onMouseButton(int button, int action, int mods) {

}

void entre_portais::TriObject::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        logger_.getLogger()->info("Hi, I'm a triangle! And my name is {}", getName());
        logger_.getLogger()->info("My position is ({}, {}, {})", transform_.x, transform_.y, transform_.z);
        logger_.getLogger()->info("My rotation is ({}, {}, {})", transform_.rx, transform_.ry, transform_.rz);
        logger_.getLogger()->info("My scale is ({}, {}, {})", transform_.sx, transform_.sy, transform_.sz);
        auto scene = getScene();
        logger_.getLogger()->info("My scene is {}", scene);
    }
    if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
        auto tm = entre_portais::TaskManager::getInstance();
        auto a = tm->addTask<entre_portais::WaitAndFinishTask>();
    }
}

void entre_portais::TriObject::CustomImGui() {
    ImGui::SliderFloat("Velocity", &velocity_, 0.0f, 200.0f);
}
