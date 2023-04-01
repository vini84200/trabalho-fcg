#include "labirinto/MenuScene.hpp"
#include "labirinto/HelpScene.hpp"
#include "labirinto/CreditsScene.hpp"
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
    center->setWitdhtConstraint(entre_portais::FixedScale(536.0f));
    center->setHeightConstraint(entre_portais::FixedScale(357.0f));
    center->setZIndex(-0.3f);

    auto start = std::make_shared<entre_portais::GuiRectangle>("Start", "ui/jogar.png");
    start->setHoverTexturePath("ui/JogarH.png");
    start->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    start->setY(entre_portais::FixedToAnchorPosition(-54.0f, entre_portais::CENTER));
    start->setWitdhtConstraint(entre_portais::FixedScale(288.0f));
    start->setHeightConstraint(entre_portais::FixedScale(54.0f));
    start->setZIndex(-0.4f);
    start->registerClickCallback([this](float x, float y) {
        this->getWindow()->setScene<GameScene>();
        this->getWindow()->showCursor(false);
    });

    auto help = std::make_shared<entre_portais::GuiRectangle>("Help", "ui/ajuda.png");
    help->setHoverTexturePath("ui/AjudaH.png");
    help->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    help->setY(entre_portais::FixedToAnchorPosition(6.0f, entre_portais::CENTER));
    help->setWitdhtConstraint(entre_portais::FixedScale(288.0f));
    help->setHeightConstraint(entre_portais::FixedScale(54.0f));
    help->setZIndex(-0.4f);
    help->registerClickCallback([this](float x, float y) {
        this->getWindow()->setScene<HelpScene>();
    });

    auto credits = std::make_shared<entre_portais::GuiRectangle>("Credits", "ui/creditos.png");
    credits->setHoverTexturePath("ui/CreditosH.png");
    credits->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    credits->setY(entre_portais::FixedToAnchorPosition(66.0f, entre_portais::CENTER));
    credits->setWitdhtConstraint(entre_portais::FixedScale(288.0f));
    credits->setHeightConstraint(entre_portais::FixedScale(54.0f));
    credits->setZIndex(-0.4f);
    credits->registerClickCallback([this](float x, float y) {
        this->getWindow()->setScene<CreditsScene>();
    });

    auto exit = std::make_shared<entre_portais::GuiRectangle>("Exit", "ui/sair.png");
    exit->setHoverTexturePath("ui/SairH.png");
    exit->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    exit->setY(entre_portais::FixedToAnchorPosition(126.0f, entre_portais::CENTER));
    exit->setWitdhtConstraint(entre_portais::FixedScale(288.0f));
    exit->setHeightConstraint(entre_portais::FixedScale(54.0f));
    exit->setZIndex(-0.4f);
    exit->registerClickCallback([this](float x, float y) {
        this->getWindow()->setRunning(false);
    });

    auto background = std::make_shared<entre_portais::GuiRectangle>("Background", "ui/menu.png");
    background->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    background->setY(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
    background->setWitdhtConstraint(entre_portais::RelativeScale(1.0f));
    background->setHeightConstraint(entre_portais::RelativeScale(1.0f));
    background->setZIndex(-0.2f);


    addChild(start);
    addChild(help);
    addChild(credits);
    addChild(exit);
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
