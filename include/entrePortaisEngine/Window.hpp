#ifndef ENTREPORTAIS_WINDOW_HPP
#define ENTREPORTAIS_WINDOW_HPP

#include "glad/glad.h"
#include "IScene.hpp"
#include "entrePortaisEngine/plugins/ImGuiPlugin.hpp"
#include <GLFW/glfw3.h>

#define VSYNC 1
#define DEFAULT_FPS 60
#define WARNING_FPS 28
#define TARGET_UPS 180.0f

namespace entre_portais {

    class Window : public std::enable_shared_from_this<Window> {
    public:
        Window(int width, int height, const char *title, std::shared_ptr<IScene> scene);

        ~Window();

        Window(const Window &other) = delete;

        Window &operator=(const Window &other) = delete;

        Window(Window &&other) = delete;

        Window &operator=(Window &&other) = delete;

        void Run();

        void RegisterPlugin(std::shared_ptr<IPlugin> component);

        void UnregisterPlugin(std::shared_ptr<IPlugin> component);

        void UnregisterAllPlugins();

        GLFWwindow *GetGLFWwindow() const;

        std::shared_ptr<IScene> GetScene() const;

        void showCursor(bool show);

        bool isCursorVisible();

    protected:
        void update(float deltaTime);

        void render();

        void onResize(int width, int height);

        void onKey(int key, int scancode, int action, int mods);

        void onMouseButton(int button, int action, int mods);

        static void onError(int error, const char *description);

        void onCloseWindow();

        void onExit();

        std::shared_ptr<spdlog::logger> getLogger() const {
            return logger_.getLogger();
        };

        void onMouseMovement(double xpos, double ypos);

        void onMouseDeltaMovement(glm::vec2 delta);

    private:
        GLFWwindow *window_;
        bool running_;
        int width_;
        int height_;
        char *title_;
        std::shared_ptr<IScene> scene_;
        std::vector<std::shared_ptr<IPlugin>> registeredPlugins_;
        double targetFPS_ = DEFAULT_FPS;
        double lastFrameTime_ = 0.0f;
        Logger logger_ = Logger("Window");
        glm::vec2 mousePos_;
        bool cursorIsVisible_;
    };
}
#endif  // ENTREPORTAIS_WINDOW_HPP
