#include "labirinto/Pillar.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "labirinto/InvisibleBox.hpp"

namespace labirinto {
    void Pillar::onKey(int key, int scancode, int action, int mods) {

    }

    void Pillar::onMouseButton(int button, int action, int mods) {

    }

    void Pillar::onExit() {

    }

    Pillar::Pillar(char *path, char *name) : IObject(name) {
        mesh_ = std::make_shared<entre_portais::MeshFromObj>(path);

    }


    void Pillar::update(float deltaTime) {
    }

    void Pillar::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("phong_triplanar");
        submit(renderer, entre_portais::RenderPass::FOREGROUND);
    }

    void Pillar::onResize(int width, int height) {
    }
} // labirinto