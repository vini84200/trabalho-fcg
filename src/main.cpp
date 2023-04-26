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
    std::string const title = "Entre Portais";


    auto load_scene = std::make_shared<labirinto::LoadingScene>();
    std::shared_ptr<entre_portais::Window> janela(
            new entre_portais::Window(WIDTH, HEIGHT, title.c_str(), std::move(load_scene)));

    entre_portais::TaskManager::getInstance();

    // Preload the textures for the loading scene
//    entre_portais::TextureManager::instance().LoadTexture("ui/loadingBkg.jpg");
//    entre_portais::TextureManager::instance().LoadTexture("ui/loadingTxt.png");
//
    janela->RegisterPlugin(std::make_shared<entre_portais::ImGuiPlugin>());
//
//    // Wait for the textures to be loaded
//    while (entre_portais::TextureManager::instance().getTexture("ui/loadingBkg.jpg").isLoaded() == false ||
//           entre_portais::TextureManager::instance().getTexture("ui/loadingTxt.png").isLoaded() == false) {
//        // Sleep for 5ms
//        spdlog::info("Waiting for textures to be loaded");
//        std::this_thread::sleep_for(std::chrono::milliseconds(5));
//    }

    // Run all the sync tasks
//    entre_portais::TaskManager::getInstance()->RunSyncTasks(10);

    janela->Run();

    return 0;
}
