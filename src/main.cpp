#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "entrePortais/Window.hpp"

void ErrorCallback(int error, const char *description) {
    printf("ERROR: GLFW: %s\n", description);
    exit(-1);
}

int main() {
    printf("Hello World!\n");
    entre_portais::Window janela(800, 600, "Hello World");
    janela.Run();

    return 0;
}

