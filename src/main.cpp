#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "entrePortais/Window.hpp"
#include "entrePortais/TriScene.hpp"

int main() {
    printf("Hello World!\n");
    char title[] = "Entre Portais";
    entre_portais::TriScene scene;
    entre_portais::Window janela(1920, 1080, title, &scene);
    janela.Run();

    return 0;
}

