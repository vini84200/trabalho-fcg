
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
        obj->initialize();
        addChild(obj);
    }

    TriScene::~TriScene() {
    }

    void TriScene::update() {

    }

    void TriScene::render() {
    }

    void TriScene::onExit() {

    }

    void TriScene::onKey(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {

    }

    void TriScene::onResize(int /*width*/, int /*height*/) {

    }

    void TriScene::onMouseButton(int /*button*/, int /*action*/, int /*mods*/) {

    }
} // entre_portais