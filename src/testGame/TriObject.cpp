#include "testGame/TriObject.hpp"
#include "entrePortaisEngine/EasyMesh.hpp"
#include "GLFW/glfw3.h"

entre_portais::TriObject::TriObject() {
    auto vert = new entre_portais::ManyVertices();
    vert->vertices.push_back({-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f});
    vert->vertices.push_back({0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f});
    vert->vertices.push_back({0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f});
    vert->indices.push_back(0);
    vert->indices.push_back(1);
    vert->indices.push_back(2);
    mesh_ = std::make_shared<EasyMesh>(*vert, "assets/shaders/tri.vert", "assets/shaders/tri.frag");

    delete vert;
    Transform t = Transform();
    t.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    t.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    t.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    transform_ = t;
}

void entre_portais::TriObject::update() {
    // Gira o triângulo
    transform_.setRotation(glm::vec3(0.0f, 0.0f, transform_.rz + 0.3f));
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
        printf("Hi, I'm a triangle! And my scene is %d\n", scene_.lock().get());
    }
}
