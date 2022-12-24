#ifndef PROJETOCG_WINDOW_HPP
#define PROJETOCG_WINDOW_HPP

#include "glad/glad.h"
#include "IScene.hpp"
#include <GLFW/glfw3.h>

namespace entre_portais {
    class Window {
    public:
        Window(int width, int height, char *title, IScene *scene);

        ~Window();

        void Run();

    protected:
        void update();

        void render();

        void onResize(int width, int height);

        void onKey(int key, int scancode, int action, int mods);

        void onMouseButton(int button, int action, int mods);

        static void onError(int error, const char *description);

        void onCloseWindow();

        void onExit();

    private:
        GLFWwindow *window_;
        bool running_;
        int width_;
        int height_;
        char *title_;
        IScene *scene_;
        double targetFPS_ = 60.0;
    };
}
#endif  // PROJETOCG_WINDOW_HPP
