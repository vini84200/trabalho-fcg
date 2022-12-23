#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/TriScene.hpp"

int main() {
    char title[] = "Entre Portais";
    entre_portais::TriScene scene;
    entre_portais::Window janela(800, 600, title, &scene);
    janela.Run();

    return 0;
}

