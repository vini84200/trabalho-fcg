#include "labirinto/LoadingScene.hpp"
#include "entrePortaisEngine/gui/GuiRectangle.hpp"
#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/render/TextureManager.hpp"

// Lista de texturas a serem carregadas



namespace labirinto {
    LoadingScene::LoadingScene() : IScene("LoadingScene") {

    }

    void LoadingScene::update(float deltaTime) {
        // Wait for the screen to be drawn on the first frame
        if (getWindow()->getFrameCount() > 3) {
            // Load the textures of the game
            entre_portais::TextureManager &textureManager = entre_portais::TextureManager::instance();
            textureManager.LoadTexture("StoneBricksBeige015_COL_2K.jpg");
            //TODO: Load the rest of the textures
        }
    }

    void LoadingScene::render() {
        IScene::render();
    }

    void LoadingScene::initialize() {
        // Loading Text
        auto loadingText = std::make_shared<entre_portais::GuiRectangle>("LoadingText",
                                                                         glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        loadingText->setTexturePath("ui/loadingTxt.png");
        loadingText->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
        loadingText->setY(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
        loadingText->setWitdhtConstraint(entre_portais::FixedScale(200.0f));
        loadingText->setHeightConstraint(entre_portais::FixedScale(50.0f));
        loadingText->setZIndex(-0.3f);

        addChild(loadingText);
        // Background
        auto background = std::make_shared<entre_portais::GuiRectangle>("LoadingBackground",
                                                                        glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        background->setTexturePath("ui/loadingBkg.jpg");
        background->setX(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
        background->setY(entre_portais::FixedToAnchorPosition(0.0f, entre_portais::CENTER));
        background->setWitdhtConstraint(entre_portais::RelativeScale(1.0f));
        background->setHeightConstraint(entre_portais::RelativeScale(1.0f));
        background->setZIndex(0.4f);

        addChild(background);


    }

    void LoadingScene::onKey(int key, int scancode, int action, int mods) {

    }

    void LoadingScene::onMouseButton(int button, int action, int mods) {

    }

    void LoadingScene::onExit() {

    }
} // labirinto