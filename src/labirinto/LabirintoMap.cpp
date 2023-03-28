#include "labirinto/LabirintoMap.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "labirinto/InvisibleBox.hpp"
#include "glm/gtx/string_cast.hpp"

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
        auto playerPosition = getPlayerPosition();
        int index = posToIndex(playerPosition);
        if (index_ != index) {
            spdlog::info("Player moved from {} to {}", index_, index);
            buildCollision();
        }
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

        buildInitialCollision();
        buildCollision();

//        for (int x = 0; x < 80; x += 2) {
//            for (int y = 0; y < 80; y += 2) {
//                auto box = std::make_shared<InvisibleBox>(x-39, y-39);
//                addChild(box);
//            }
//        }

        // Add lights

    }

    std::vector<glm::vec3> nearestWalls(glm::vec3 position) {

    }

    void LabirintoMap::onResize(int width, int height) {
    }

    glm::vec2 LabirintoMap::getPlayerPosition() {
        // Retorna a posição do jogador
        auto playerPosition = getParent()->getScene()->getCamera()->getParent()->getParent()->getParent()->getTransform()->getPosition();
        return glm::vec2(playerPosition.x, playerPosition.z);
    }

    int LabirintoMap::posToIndex(glm::vec2 position) {
        float xFloat = position.x, yFloat = position.y;
        xFloat += 40;
        yFloat += 40;
        xFloat /= 2;
        yFloat /= 2;
        int x = static_cast<int>(trunc(xFloat));
        int y = static_cast<int>(trunc(yFloat));
        return (x + 41 * y);
    }

    std::vector<int> LabirintoMap::indexSquare(int index) {
        auto playerPosition = getPlayerPosition();
        std::vector<int> indexes;
        // TODO: Checar out of bouds
        indexes.push_back(index - 42);
        indexes.push_back(index - 41);
        indexes.push_back(index - 40);
        indexes.push_back(index - 1);
        indexes.push_back(index + 1);
        indexes.push_back(index + 40);
        indexes.push_back(index + 41);
        indexes.push_back(index + 42);
        indexes.push_back(index);
        return indexes;
    }

    void LabirintoMap::indexesDraw(std::vector<int> indexes) {
        glm::vec2 positionToDraw;
        for (int i = 0; i < indexes.size(); i++) {
            // indexes é um vetor de indices
            if (array_[indexes.at(i)]) {
                positionToDraw = indexToPos(indexes.at(i));
                auto box = std::make_shared<InvisibleBox>(positionToDraw.x, positionToDraw.y);
                addChild(box);
            }
        }
    }

    glm::vec2 LabirintoMap::indexToPos(int index) {
        int x = index % 41;
        int y = index / 41;
        x *= 2;
        y *= 2;
        x -= 40;
        y -= 40;
//        if (x % 2 == 0){
//            x++;
//        }
//        if (y % 2 == 0){
//            y++;
//        }
//        x++;
//        y++;
        glm::vec2 position = glm::vec2(x, y);
        return position;
    }

    void LabirintoMap::buildCollision() {
        auto playerPosition = getPlayerPosition();
        int index = posToIndex(playerPosition);
        index_ = index;
        std::vector<int> indexes = indexSquare(index);
        // indexesDraw(indexes);
        for (int i = 0; i < 9; i++) {
            // TODO: Checar out of bouds
            // if (indexes.count(i) == 0) {
            //     colliders_[i]->getTransform()->setPosition(glm::vec3(-40, -40, 0));
            //     continue;
            // }
            if (array_[indexes.at(i)]) {
                glm::vec2 positionToDraw = indexToPos(indexes.at(i));
                colliders_[i]->getTransform()->setPosition(glm::vec3(positionToDraw.x, 0, positionToDraw.y));
                spdlog::info("Collider {} at {}", i, glm::to_string(indexToPos(indexes.at(i))));
            } else {
                // colliders_[i]->getTransform()->setPosition(glm::vec3(-40, -40, 0));
                // spdlog::info("Collider {} at {}", i, glm::to_string(glm::vec2(-40, -40)));
            }
        }
    }

    void LabirintoMap::buildInitialCollision() {
        // Create the object that will be used to check the collision
        // outside the map
        for (int i = 0; i < 9; i++) {
            auto box = std::make_shared<InvisibleBox>(-40, -40);
            box->getTransform()->setPosition(glm::vec3(-40, -40, 0));
            addChild(box);
            colliders_[i] = box;
        }
    }
} // labirinto