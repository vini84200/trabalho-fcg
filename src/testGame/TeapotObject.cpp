#include "testGame/TeapotObject.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"

namespace entre_portais {
    void TeapotObject::onKey(int key, int scancode, int action, int mods) {

    }

    void TeapotObject::onMouseButton(int button, int action, int mods) {

    }

    void TeapotObject::onExit() {

    }

    void TeapotObject::update(float deltaTime) {
    }

    void TeapotObject::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("gouraud");
        submit(renderer, RenderPass::FOREGROUND);
    }

    void TeapotObject::onResize(int width, int height) {
    }

    TeapotObject::TeapotObject(char *name) : IObject(name) {
        mesh_ = std::make_shared<MeshFromObj>("assets/objs/hinata4.obj");
        transform_.setScale({1, 1, 1});
        transform_.setPosition({0, 1, 0});

    }
} // entre_portais