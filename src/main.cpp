#include <fstream>
#include <sstream>
#include <string>

#include "entrePortaisEngine/Logger.hpp"
#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "testGame/TriScene.hpp"
#include "labirinto/GameScene.hpp"

static const int WIDTH = 800;
static const int HEIGHT = 600;

int main() {
    entre_portais::Logger::initialize();
    std::string title = "Entre Portais";
    auto scene = std::make_shared<labirinto::GameScene>();
    std::shared_ptr<entre_portais::Window> janela(
            new entre_portais::Window(WIDTH, HEIGHT, title.c_str(), std::move(scene)));

    entre_portais::TaskManager::getInstance();

    janela->RegisterPlugin(std::make_shared<entre_portais::ImGuiPlugin>());
    janela->Run();

    return 0;
}
