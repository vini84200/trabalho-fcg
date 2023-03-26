#include "LabirintoMap.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"

namespace labirinto {
    void LabirintoMap::onKey(int key, int scancode, int action, int mods) {

    }

    void LabirintoMap::onMouseButton(int button, int action, int mods) {

    }

    void LabirintoMap::onExit() {

    }

    LabirintoMap::LabirintoMap() : IObject("LabirintoMap") {
        mesh_ = std::make_shared<entre_portais::MeshFromObj>("assets/objs/labirinto.obj");

    }


    void LabirintoMap::update(float deltaTime) {
    }

    void LabirintoMap::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("phong_triplanar");
        submit(renderer, entre_portais::RenderPass::FOREGROUND);

    }

    void LabirintoMap::onResize(int width, int height) {
    }
} // labirinto