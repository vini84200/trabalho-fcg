#include "labirinto/LoadingScene.hpp"
#include "entrePortaisEngine/gui/GuiRectangle.hpp"
#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/render/TextureManager.hpp"
#include "labirinto/GameScene.hpp"
#include "labirinto/MenuScene.hpp"

// Lista de texturas a serem carregadas



namespace labirinto {
    LoadingScene::LoadingScene() : IScene("LoadingScene") {

    }


    void LoadingScene::update(float deltaTime) {

        // Wait for the screen to be drawn on the first frame
        if (getWindow()->getFrameCount() > 3) {
            if (texturePreloadQueue.empty()) {
                // Start timer to load the next scene
                finishTimer_ += deltaTime;
            } else {
                // Load the next texture
                auto texturePath = texturePreloadQueue.front();
                texturePreloadQueue.pop();
                entre_portais::TextureManager::instance().LoadTexture(texturePath);
            }
        }

        if (finishTimer_ > 1.2f) {
            // Load the next scene
            getWindow()->setScene<labirinto::MenuScene>();
        }
    }

    void LoadingScene::render() {
        IScene::render();
    }

    void LoadingScene::initialize() {

        // Put the textures to be loaded in the queue
        texturePreloadQueue.push("ui/loadingBkg.jpg");
        texturePreloadQueue.push("ui/loadingTxt.png");
        texturePreloadQueue.push("skin.png");
        texturePreloadQueue.push("hair.png");
        texturePreloadQueue.push("StoneBricksBeige015_COL_2K.jpg");
        texturePreloadQueue.push("ui/ajuda.png");
        texturePreloadQueue.push("ui/AjudaH.png");
        texturePreloadQueue.push("ui/creditos.png");
        texturePreloadQueue.push("ui/CreditosH.png");
        texturePreloadQueue.push("ui/credits_info.png");
        texturePreloadQueue.push("ui/help_info.png");
        texturePreloadQueue.push("ui/jogar.png");
        texturePreloadQueue.push("ui/jogarH.png");
        texturePreloadQueue.push("ui/menu.png");
        texturePreloadQueue.push("ui/menu_box.png");
        texturePreloadQueue.push("ui/menu_principal.png");
        texturePreloadQueue.push("ui/Menu_principalH.png");
        texturePreloadQueue.push("ui/sair.png");
        texturePreloadQueue.push("ui/SairH.png");
        texturePreloadQueue.push("ui/voltar.png");
        texturePreloadQueue.push("ui/VoltarH.png");
        texturePreloadQueue.push("ui/win_info.png");



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