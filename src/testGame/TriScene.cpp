
#include <cstdio>
#include "testGame/TriScene.hpp"
#include "glad/glad.h"
#include "testGame/TriObject.hpp"

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
    }

    TriScene::~TriScene() {
    }

    void TriScene::update(double deltaTime) {

    }

    void TriScene::onExit() {

    }

    void TriScene::onKey(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {

    }

    void TriScene::onResize(int /*width*/, int /*height*/) {

    }

    void TriScene::onMouseButton(int /*button*/, int /*action*/, int /*mods*/) {

    }

    void TriScene::render() {

    }

} // entre_portais