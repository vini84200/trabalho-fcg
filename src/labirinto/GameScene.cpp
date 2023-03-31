#include "labirinto/GameScene.hpp"
#include "entrePortaisEngine/Objects/AmbientFromTexture.hpp"
#include "labirinto/Player.hpp"
#include "testGame/Player.hpp"
#include "entrePortaisEngine/Objects/ObjFromFile.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "entrePortaisEngine/physics/BoxCollider.hpp"
#include "labirinto/LabirintoMap.hpp"
#include "labirinto/Pillar.hpp"
#include "labirinto/Key.hpp"
#include "entrePortaisEngine/gui/GuiRectangle.hpp"
#include "testGame/TeapotObject.hpp"

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
        player->getTransform()->setPosition(glm::vec3(35.0f, 3.0f, 0.0f));
        player->initializePropagate();
        setCamera(player->getCamera());



        // Adicionar o mapa
        auto map = std::make_shared<LabirintoMap>();
        addChild(map);

        // Adicionar os objetos
        // Adicionar a chave usando instanciacao
        auto keyMesh = std::make_shared<entre_portais::MeshFromObj>("assets/objs/key.obj");
        char *nameR = "RustyRealKey";
        auto rustyKey = std::make_shared<Key>(true, nameR);
        rustyKey->setMesh(keyMesh);
        addChild(rustyKey);
        rustyKey->getTransform()->setScale(glm::vec3(0.05, 0.05, 0.05));
        rustyKey->getTransform()->setPosition(glm::vec3(-37.0, 2.4, 35.0));

        char *nameA = "RustyDeadlyKey1";
        rustyKey = std::make_shared<Key>(false, nameA);
        rustyKey->setMesh(keyMesh);
        addChild(rustyKey);
        rustyKey->getTransform()->setScale(glm::vec3(0.05, 0.05, 0.05));
        rustyKey->getTransform()->setPosition(glm::vec3(-35.0, 2.4, 35.0));

        char *nameB = "RustyDeadlyKey2";
        rustyKey = std::make_shared<Key>(false, nameB);
        rustyKey->setMesh(keyMesh);
        addChild(rustyKey);
        rustyKey->getTransform()->setScale(glm::vec3(0.05, 0.05, 0.05));
        rustyKey->getTransform()->setPosition(glm::vec3(-33.0, 2.4, 35.0));


        char *stonePillarName = "stonePillar";
        char *pathToStone = "assets/objs/stonePillar.obj";
        auto stonePillar = std::make_shared<Pillar>(pathToStone, stonePillarName);
        addChild(stonePillar);
        stonePillar->getTransform()->setPosition(glm::vec3(-37.0, 1.0, 35.0));

        char *silverPillarName = "silverPillar";
        char *pathToSilver = "assets/objs/silverPillar.obj";
        auto silverPillar = std::make_shared<Pillar>(pathToSilver, silverPillarName);
        addChild(silverPillar);
        silverPillar->getTransform()->setPosition(glm::vec3(-35.0, 1.0, 35.0));

        char *goldenPillarName = "goldenPillar";
        char *pathToGolden = "assets/objs/goldenPillar.obj";
        auto goldenPillar = std::make_shared<Pillar>(pathToGolden, goldenPillarName);
        addChild(goldenPillar);
        goldenPillar->getTransform()->setPosition(glm::vec3(-33.0, 1.0, 35.0));

        char *goldenPillarName2 = "goldenPillar2";
        auto goldenPillar2 = std::make_shared<Pillar>(pathToGolden, goldenPillarName2);
        addChild(goldenPillar2);
        goldenPillar2->getTransform()->setPosition(glm::vec3(8.0, 1.0, 10.5));

        char *goldenPillarName3 = "goldenPillar3";
        auto goldenPillar3 = std::make_shared<Pillar>(pathToGolden, goldenPillarName3);
        addChild(goldenPillar3);
        goldenPillar3->getTransform()->setPosition(glm::vec3(8.0, 1.0, 9.0));

        char *gouraudHinataName = "gouraudHinata";
        auto gouraudHinata = std::make_shared<entre_portais::TeapotObject>(gouraudHinataName);
        addChild(gouraudHinata);
        gouraudHinata->getTransform()->setPosition(glm::vec3(8.0, 1.8, 10.5));
        gouraudHinata->getTransform()->setRotationZYX(glm::vec3(0.0, M_PI_2, 0.0));
        gouraudHinata->getTransform()->setScale(glm::vec3(0.08, 0.08, 0.08));

        char *phongHinataName = "phongHinata";
        auto phongHinata = std::make_shared<entre_portais::ObjFromFile>(phongHinataName, "assets/objs/hinata4.obj");
        addChild(phongHinata);
        phongHinata->getTransform()->setPosition(glm::vec3(8.0, 1.8, 9.0));
        phongHinata->getTransform()->setRotationZYX(glm::vec3(0.0, M_PI_2, 0.0));
        phongHinata->getTransform()->setScale(glm::vec3(0.08, 0.08, 0.08));
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


        // Add UI test
        /*auto uiTest = std::make_shared<entre_portais::GuiRectangle>("uiTest", glm::vec4(119.f / 256.f, 38.f / 256.f,
                                                                                        189.f / 256.f, 1));
        uiTest->setX(entre_portais::FixedToAnchorPosition(300.0f, entre_portais::Anchor::RIGHT));
        uiTest->setY(entre_portais::FixedToAnchorPosition(70.0f, entre_portais::Anchor::BOTTOM));
        uiTest->setWitdhtConstraint(entre_portais::FixedScale(200.0f));
        uiTest->setHeightConstraint(entre_portais::FixedScale(40.0f));
        uiTest->setTexturePath("btn/Normal.png");
        uiTest->setHoverTexturePath("btn/Hovered.png");
        uiTest->setTextureSize(glm::vec2(2.0f, 4.0f));
        uiTest->setTextureOffset(glm::vec2(1.0f, 0.6f));
        auto uiTestWk = std::weak_ptr<entre_portais::GuiRectangle>(uiTest);
        uiTest->registerClickCallback([this, uiTestWk](float x, float y) {
            auto uiTest = uiTestWk.lock();
            uiTest->setTextureSize(glm::vec2(1.0f, 1.0f));
            uiTest->setTextureOffset(glm::vec2(0.0f, 0.0f));
            this->removeChild(uiTest);
        });
        addChild(uiTest);*/
    }

} // labirinto