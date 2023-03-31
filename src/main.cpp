#include <fstream>
#include <sstream>
#include <string>

#include "entrePortaisEngine/Logger.hpp"
#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "testGame/TriScene.hpp"
#include "labirinto/GameScene.hpp"
#include "labirinto/LoadingScene.hpp"
#include "entrePortaisEngine/render/TextureManager.hpp"

static const int WIDTH = 800;
static const int HEIGHT = 600;

int main() {
    entre_portais::Logger::initialize();
    std::string title = "Entre Portais";


    auto load_scene = std::make_shared<labirinto::LoadingScene>();
    std::shared_ptr<entre_portais::Window> janela(
            new entre_portais::Window(WIDTH, HEIGHT, title.c_str(), std::move(load_scene)));

    entre_portais::TaskManager::getInstance();

    // Preload the textures for the loading scene
//    entre_portais::TextureManager::instance().LoadTexture("ui/loadingBkg.jpg");
//    entre_portais::TextureManager::instance().LoadTexture("ui/loadingTxt.png");

    janela->RegisterPlugin(std::make_shared<entre_portais::ImGuiPlugin>());
    janela->Run();

    return 0;
}
