#include <fstream>
#include <sstream>
#include <string>

#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/TriScene.hpp"

static const int WIDTH = 800;
static const int HEIGHT = 600;

int main() {
    std::string title = "Entre Portais";
    std::unique_ptr<entre_portais::TriScene> scene(new entre_portais::TriScene());
    std::unique_ptr<entre_portais::Window> janela(
            new entre_portais::Window(
                    WIDTH,
                    HEIGHT,
                    title.c_str(),
                    scene.get()
            ));

    janela->Run();

    scene.reset();
    janela.reset();
    return 0;
}

