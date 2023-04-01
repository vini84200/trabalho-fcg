#include "labirinto/HelpScene.hpp"
#include "labirinto/MenuScene.hpp"
#include "entrePortaisEngine/gui/GuiRectangle.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/Window.hpp"

void labirinto::HelpScene::update(float deltaTime) {
    IScene::update(deltaTime);
}

void labirinto::HelpScene::render() {
    IScene::render();
}

void labirinto::HelpScene::initialize() {

    auto center = std::make_shared<entre_portais::GuiRectangle>("CenterText", "ui/help_info.png");
    center->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    center->setY(entre_portais::FixedToAnchorPosition(-100.0f, entre_portais::CENTER));
    center->setWitdhtConstraint(entre_portais::FixedScale(870.0f * 0.8));
    center->setHeightConstraint(entre_portais::FixedScale(432.0f * 0.8));
    center->setZIndex(-0.3f);

    auto back = std::make_shared<entre_portais::GuiRectangle>("Back", "ui/voltar.png");
    back->setHoverTexturePath("ui/VoltarH.png");
    back->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    back->setY(entre_portais::FixedToAnchorPosition(200.0f, entre_portais::CENTER));
    back->setWitdhtConstraint(entre_portais::FixedScale(288.0f));
    back->setHeightConstraint(entre_portais::FixedScale(54.0f));
    back->setZIndex(-0.4f);
    back->registerClickCallback([this](float x, float y) {
        this->getWindow()->setScene<MenuScene>();
    });

    auto background = std::make_shared<entre_portais::GuiRectangle>("Background", "ui/menu.png");
    background->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    background->setY(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    background->setWitdhtConstraint(entre_portais::RelativeScale(1.0f));
    background->setHeightConstraint(entre_portais::RelativeScale(1.0f));
    background->setZIndex(-0.2f);


    addChild(back);
    addChild(center);
    addChild(background);


}

void labirinto::HelpScene::onKey(int key, int scancode, int action, int mods) {

}

void labirinto::HelpScene::onMouseButton(int button, int action, int mods) {

}

void labirinto::HelpScene::onExit() {

}

labirinto::HelpScene::HelpScene() : IScene("HelpScene") {

}
