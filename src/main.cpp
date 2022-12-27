#include <fstream>
#include <sstream>
#include <string>

#include "entrePortaisEngine/Window.hpp"
#include "testGame/TriScene.hpp"

static const int WIDTH = 800;
static const int HEIGHT = 600;

int main() {
    std::string title = "Entre Portais";
    auto scene = std::make_shared<entre_portais::TriScene>();
    std::unique_ptr<entre_portais::Window> janela(
            new entre_portais::Window(
                    WIDTH,
                    HEIGHT,
                    title.c_str(),
                    std::move(scene)
            ));

    janela->Run();
    return 0;
}

