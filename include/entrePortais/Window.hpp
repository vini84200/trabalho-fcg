#ifndef PROJETOCG_WINDOW_HPP
#define PROJETOCG_WINDOW_HPP

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace entre_portais {
    class Window {
    public:
        Window(int width, int height, char *title);

        ~Window();

        void Run();

        void update();

        void render();

        void onResize(int width, int height);

        void onKey(int key, int scancode, int action, int mods);

        void onMouseButton(int button, int action, int mods);

        static void onError(int error, const char *description);

        void onExit();

    private:
        GLFWwindow *window_;
        bool running_;
        int width_;
        int height_;
        char *title_;

    };
}
#endif  // PROJETOCG_WINDOW_HPP
