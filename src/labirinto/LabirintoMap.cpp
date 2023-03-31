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
        xFloat *= (41.0 / 40.0);
        yFloat *= (41.0 / 40.0);
        xFloat += 39.0;
        yFloat += 39.0;
        xFloat /= 2.0;
        yFloat /= 2.0;
        int x = static_cast<int>(std::floor(xFloat));
        int y = static_cast<int>(std::floor(yFloat));
        return (x + 41 * y);
    }

    std::vector<int> LabirintoMap::indexSquare(int index) {
        auto playerPosition = getPlayerPosition();
        std::vector<int> indexes;
        if (index < 1680) {
            indexes.push_back(index);
        }
        if (index + 1 < 1680) {
            indexes.push_back(index + 1);
        }
        if (index + 2 < 1680) {
            indexes.push_back(index + 2);
        }
        if (index + 41 < 1680) {
            indexes.push_back(index + 41);
        }
        if (index + 42 < 1680) {
            indexes.push_back(index + 42);
        }
        if (index + 43 < 1680) {
            indexes.push_back(index + 43);
        }
        if (index + 82 < 1680) {
            indexes.push_back(index + 82);
        }
        if (index + 83 < 1680) {
            indexes.push_back(index + 83);
        }
        if (index + 84 < 1680) {
            indexes.push_back(index + 84);
        }
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
        int xInt = index % 41;
        int yInt = index / 41;
        float x = static_cast<float>(xInt);
        float y = static_cast<float>(yInt);
        x *= 2.0;
        y *= 2.0;
        x -= 40.0;
        y -= 40.0;
        x *= (40.0 / 41.0);
        y *= (40.0 / 41.0);
        spdlog::info("X and Y at {}, {}", x, y);
        glm::vec2 position = glm::vec2(x, y);
        return position;
    }

    void LabirintoMap::buildCollision() {
        auto playerPosition = getPlayerPosition();
        int index = posToIndex(playerPosition);
        index_ = index;
        std::vector<int> indexes = indexSquare(index);
        for (int i = 0; i < indexes.size(); i++) {
            if (array_[indexes.at(i)]) {
                glm::vec2 positionToDraw = indexToPos(indexes.at(i));
                colliders_[i]->getTransform()->setPosition(glm::vec3(positionToDraw.x, 0, positionToDraw.y));
                spdlog::info("Collider {} at {}", i, glm::to_string(indexToPos(indexes.at(i))));
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