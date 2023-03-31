#include "testGame/Cuelho.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"

namespace entre_portais {
    void Cuelho::onKey(int key, int scancode, int action, int mods) {

    }

    void Cuelho::onMouseButton(int button, int action, int mods) {

    }

    void Cuelho::onExit() {

    }

    void Cuelho::update(float deltaTime) {
    }

    void Cuelho::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("phong");
        submit(renderer, RenderPass::FOREGROUND);
    }

    void Cuelho::onResize(int width, int height) {
    }

    Cuelho::Cuelho(char *name) : IObject(name) {
        mesh_ = std::make_shared<MeshFromObj>("assets/objs/hinata4.obj");
        transform_.setScale({1, 1, 1});
        transform_.setPosition({0, 1, 0});
    }
} // entre_portais