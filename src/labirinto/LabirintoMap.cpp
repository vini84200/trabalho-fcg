#include "labirinto/LabirintoMap.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "labirinto/InvisibleBox.hpp"

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

        // Create all the Colliders for the map
        auto grndBox = std::make_shared<InvisibleBox>(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(80.0f, 1.0f, 80.0f),
                "Ground Collider"
        );
        addChild(grndBox);

//        for (int x = 0; x < 40; x++) {
//            for (int y = 0; y < 40; y++) {
//                if (x + y % 3 == 0) {
//                    continue;
//                }
//                auto box = std::make_shared<InvisibleBox>(x-20, y-20);
//                addChild(box);
//            }
//        }

        // Add lights

    }

    void LabirintoMap::onResize(int width, int height) {
    }
} // labirinto