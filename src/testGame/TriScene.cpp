
#include <cstdio>
#include "testGame/TriScene.hpp"
#include "glad/glad.h"
#include "testGame/TriObject.hpp"

namespace entre_portais {
    TriScene::TriScene() {
    }

    void TriScene::initialize() {
        printf("Initializing TriScene\n");
        auto obj = std::make_shared<TriObject>();
        auto obj2 = std::make_shared<TriObject>();
        obj->initialize();
        addChild(obj);
        obj2->getTransform()->setPosition(glm::vec3(0.3f, 0.3f, 0.0f));
        obj2->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
        addChild(obj2);
    }

    TriScene::~TriScene() {
    }

    void TriScene::update() {

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