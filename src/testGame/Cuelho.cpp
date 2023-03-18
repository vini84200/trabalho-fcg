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
        loadShader("blinn-phong");
        submit(renderer);
    }

    void Cuelho::onResize(int width, int height) {
    }

    Cuelho::Cuelho(char *name) : IObject(name) {
        mesh_ = std::make_shared<MeshFromObj>("assets/objs/Intercity1.obj");
        transform_.setPosition({1.24, 0.86, -1.06});
        transform_.setRotation({0, -.36, 0});
    }
} // entre_portais