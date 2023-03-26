#include "labirinto/GameScene.hpp"
#include "entrePortaisEngine/Objects/AmbientFromTexture.hpp"
#include "Player.hpp"
#include "testGame/Player.hpp"
#include "entrePortaisEngine/Objects/ObjFromFile.hpp"
#include "entrePortaisEngine/physics/BoxCollider.hpp"

namespace labirinto {
    void GameScene::CustomImGui() {
    }

    void GameScene::onKey(int key, int scancode, int action, int mods) {

    }

    void GameScene::onMouseButton(int button, int action, int mods) {

    }

    void GameScene::onExit() {

    }

    GameScene::GameScene() : IScene("GameScene") {

    }

    void GameScene::update(float deltaTime) {
        IScene::update(deltaTime);
    }

    void GameScene::render() {
        IScene::render();
    }

    void GameScene::initialize() {
        auto skybox = std::make_shared<entre_portais::AmbientFromTexture>(std::array<std::string, 6>({
                                                                                                             "assets/textures/new_skybox/px.png",
                                                                                                             "assets/textures/new_skybox/nx.png",
                                                                                                             "assets/textures/new_skybox/py.png",
                                                                                                             "assets/textures/new_skybox/ny.png",
                                                                                                             "assets/textures/new_skybox/pz.png",
                                                                                                             "assets/textures/new_skybox/nz.png"
                                                                                                     }));
        addChild(skybox);

        char *name = "Player";
        auto player = std::make_shared<labirinto::Player>(name);
        addChild(player);
        player->getTransform()->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
        player->initializePropagate();
        setCamera(player->getCamera());



        // Adicionar o mapa

        // Adicionar os objetos

        // Adicionar o objetivo

        auto grnd = std::make_shared<entre_portais::ObjFromFile>("Wood Plane 2", "assets/objs/wood_plane12.obj");
        addChild(grnd);
        grnd->getTransform()->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
        auto grndCollider = std::make_unique<entre_portais::BoxCollider>(glm::vec3(2.0f, 0.2f, 2.0f),
                                                                         grnd->getModelMatrix());
        grnd->setRigidBody(std::make_unique<entre_portais::RigidBody>(&grnd->getModelMatrix(), std::move(grndCollider),
                                                                      *getPhysicsEngine().get(),
                                                                      *grnd->getTransform()));
        grnd->getRigidBody()->setIsStatic(true);
    }

} // labirinto