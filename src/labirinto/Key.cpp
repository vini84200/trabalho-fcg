#include "labirinto/Key.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "labirinto/InvisibleBox.hpp"

namespace labirinto {
    void Key::onKey(int key, int scancode, int action, int mods) {

    }

    void Key::onMouseButton(int button, int action, int mods) {

    }

    void Key::onExit() {

    }

    Key::Key(bool isDoor, char *name) : IObject(name), isDoor_(isDoor) {

    }


    void Key::update(float deltaTime) {
    }

    void Key::initialize() {
        // mesh_ = std::make_shared<entre_portais::MeshFromObj>(path);
        if (mesh_.get() == nullptr) {
            spdlog::error("Mesh is null at Key, should have been set before calling initialize");
        }
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("phong");
        submit(renderer, entre_portais::RenderPass::FOREGROUND);
    }

    void Key::onResize(int width, int height) {
    }
} // labirinto