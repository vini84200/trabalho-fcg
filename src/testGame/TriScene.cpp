
#include <cstdio>
#include "testGame/TriScene.hpp"

#include "entrePortaisEngine/render/Camera.hpp"
#include "glad/glad.h"
#include "testGame/TriObject.hpp"
#include "testGame/CubeObject.hpp"
#include "testGame/CylinderObject.hpp"
#include "testGame/Ground.hpp"
#include "testGame/Player.hpp"
#include "testGame/TeapotObject.hpp"
#include "testGame/Cuelho.hpp"
#include "entrePortaisEngine/Objects/ObjFromFile.hpp"

namespace entre_portais {
    TriScene::TriScene() : IScene("TriScene") {
    }

    void TriScene::initialize() {
        logger_.getLogger()->info("Initializing TriScene");
        char *name1 = "TriObject 1";
        char *name2 = "TriObject 2";
        auto obj = std::make_shared<TriObject>(name1);
        auto obj2 = std::make_shared<TriObject>(name2);
        addChild(obj);
        obj2->getTransform()->setPosition(glm::vec3(0.3f, 0.3f, 0.0f));
        obj2->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
        addChild(obj2);

        char *name3 = "Child of TriObject 2";
        auto obj3 = std::make_shared<TriObject>(name3);
        obj3->getTransform()->setPosition(glm::vec3(0.3f, 0.3f, 0.0f));
        obj3->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
        obj2->addChild(obj3);

        char *name4 = "Child of Child of TriObject 2";
        auto obj4 = std::make_shared<TriObject>(name4);
        obj4->getTransform()->setPosition(glm::vec3(0.3f, 0.3f, 0.0f));
        obj4->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
        obj3->addChild(obj4);

        char *cuboname = "Cubo";
        auto cube = std::make_shared<CubeObject>(cuboname);
        cube->getTransform()->setPosition(glm::vec3(0.0f, 0.5f, 0.0f));
        addChild(cube);

        char *cylinname = "Cilindro";
        auto cylin = std::make_shared<CylinderObject>(cylinname);
        addChild(cylin);

        auto grnd = std::make_shared<Ground>();
        addChild(grnd);

        char *teapotName = "Teapot";
        auto teapot = std::make_shared<TeapotObject>(teapotName);
        addChild(teapot);

        char *cuelhoNome = "Cuelho";
        auto cuelho = std::make_shared<Cuelho>(cuelhoNome);
        addChild(cuelho);

        char *path = "assets/objs/Intercity1.obj";
        char *name = "Train";
        auto train = std::make_shared<ObjFromFile>(name, path);
        addChild(train);

        addChild(std::make_shared<ObjFromFile>("Bunny", "assets/objs/bunny.obj"));


        char *charName = "Leoncio";
        auto character = std::make_shared<Player>(charName);
        addChild(character);
        character->initializePropagate();
        setCamera(character->getCamera());
    }

    TriScene::~TriScene() = default;

    void TriScene::update(float deltaTime) {
        IScene::update(deltaTime);
    }

    void TriScene::onExit() {

    }

    void TriScene::onKey(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {

    }

    void TriScene::onResize(int /*width*/, int /*height*/) {

    }

    void TriScene::onMouseButton(int /*button*/, int /*action*/, int /*mods*/) {

    }

}  // namespace entre_portais