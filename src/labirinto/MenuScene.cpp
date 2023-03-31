#include "labirinto/MenuScene.hpp"
#include "entrePortaisEngine/gui/GuiRectangle.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/Window.hpp"
#include "labirinto/GameScene.hpp"

void labirinto::MenuScene::update(float deltaTime) {
    IScene::update(deltaTime);
}

void labirinto::MenuScene::render() {
    IScene::render();
}

void labirinto::MenuScene::initialize() {

    auto center = std::make_shared<entre_portais::GuiRectangle>("CenterText", "ui/menu_box.png");
    center->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    center->setY(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    center->setWitdhtConstraint(entre_portais::FixedScale(537.0f));
    center->setHeightConstraint(entre_portais::FixedScale(313.0f));
    center->setZIndex(-0.3f);

    auto start = std::make_shared<entre_portais::GuiRectangle>("Start", "ui/jogar.png");
    start->setHoverTexturePath("ui/JogarH.png");
    start->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    start->setY(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    start->setWitdhtConstraint(entre_portais::FixedScale(288.0f));
    start->setHeightConstraint(entre_portais::FixedScale(54.0f));
    start->setZIndex(-0.4f);
    start->registerClickCallback([this](float x, float y) {
        this->getWindow()->setScene<GameScene>();
    });

    auto background = std::make_shared<entre_portais::GuiRectangle>("Background", "ui/menu.png");
    background->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    background->setY(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    background->setWitdhtConstraint(entre_portais::RelativeScale(1.0f));
    background->setHeightConstraint(entre_portais::RelativeScale(1.0f));
    background->setZIndex(-0.2f);

    addChild(start);
    addChild(center);
    addChild(background);


}

void labirinto::MenuScene::onKey(int key, int scancode, int action, int mods) {

}

void labirinto::MenuScene::onMouseButton(int button, int action, int mods) {

}

void labirinto::MenuScene::onExit() {

}

labirinto::MenuScene::MenuScene() : IScene("MenuScene") {

}
