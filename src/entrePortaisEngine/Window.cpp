#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/Logger.hpp"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

namespace entre_portais {
    Window::Window(int width, int height, const char *title, std::shared_ptr<IScene> scene) {

        if (scene == NULL) {
            fprintf(stderr, "ERROR: scene is a null pointer.\n");
            std::exit(EXIT_FAILURE);
        }
        width_ = width;
        height_ = height;
        title_ = (char *) title;
        running_ = true;
        scene_ = scene;
        int success = glfwInit();
        if (!success) {
            fprintf(stderr, "ERROR: glfwInit() failed.\n");
            std::exit(EXIT_FAILURE);
        }
        glfwSetErrorCallback(onError);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window_ = glfwCreateWindow(width_, height_, title_, NULL, NULL);

        if (!window_) {
            log("glfwTerminate()");
            glfwTerminate();
            fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
            std::exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window_);
        glfwSetWindowUserPointer(window_, this);
        glfwSetWindowSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
            Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
            self->onResize(width, height);
        });
        glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
            self->onKey(key, scancode, action, mods);
        });
        glfwSetMouseButtonCallback(window_, [](GLFWwindow *window, int button, int action, int mods) {
            Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
            self->onMouseButton(button, action, mods);
        });
        glfwSetWindowCloseCallback(window_, [](GLFWwindow *window) {
            Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
            self->onCloseWindow();
        });;

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            fprintf(stderr, "ERROR: gladLoadGLLoader() failed.\n");
            std::exit(EXIT_FAILURE);
        }
        glfwSwapInterval(1);
        scene_->initialize();
    }

    Window::~Window() {
        log("Window::~Window()");
        scene_.reset();
        log("glfwTerminate()");
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    void Window::Run() {
        lastFrameTime_ = glfwGetTime() - 1.0 / targetFPS_;
        while (running_) {
            double currentFrame = glfwGetTime();
            double deltaTime = currentFrame - lastFrameTime_;
            lastFrameTime_ = currentFrame;
            render();
            update(deltaTime);
            glfwSwapBuffers(window_);
            glfwPollEvents();
            if (deltaTime < 1.0 / targetFPS_) {
                // Here we can do some work while waiting for the next frame.

                // Sleep for the remaining time.
                usleep((1.0f / targetFPS_ - deltaTime) * 1000000);
            }
        }
        scene_->onExit();
        onExit();
    }

    void Window::update(double deltaTime) {
        for (auto &plugin: registeredPlugins_) {
            plugin->update(deltaTime);
        }
        scene_->updatePropagate(deltaTime);
    }

    void Window::render() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        scene_->renderPropagate();
    }

    void Window::onResize(int width, int height) {
        width_ = width;
        height_ = height;
        printf("Window resized to %dx%d.\n", width_, height_);
        scene_->resize(width_, height_);
    }

    void Window::onKey(int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            running_ = false;
        } else {
            printf("Key %d %s.\n", key, action == GLFW_PRESS ? "pressed" : "released");
            scene_->keyPress(key, scancode, action, mods);
        }
    }

    void Window::onMouseButton(int button, int action, int mods) {
        log("Pressed button");
        log("action:", action);
        log("mods:", mods);
        scene_->mouseButton(button, action, mods);
    }

    void Window::onError(int /*error*/, const char *description) {
        fprintf(stderr, "ERROR: %s\n", description);
    }

    void Window::onCloseWindow() {
        running_ = false;
    }

    void Window::onExit() {
        printf("Bye bye! See you soon...\n");
        scene_->exit();
        UnregisterAllComponents();
    }

    void Window::RegisterPlugin(std::shared_ptr<IPlugin> plugin) {
        registeredPlugins_.push_back(plugin);
        plugin->SetWindow(shared_from_this());
        plugin->onAttach();
    }

    void Window::UnregisterPlugin(std::shared_ptr<IPlugin> plugin) {
        printf("Unregistering component %p.\n", plugin.get());
        printf("ERROR: UnregisterPlugin() not implemented.\n");
        std::exit(EXIT_FAILURE);
    }

    void Window::UnregisterAllPlugins() {
        while (!registeredPlugins_.empty()) {
            auto a = registeredPlugins_.back();
            a->onDetach();
            registeredPlugins_.pop_back();
        }
    }

    GLFWwindow *Window::GetGLFWwindow() const {
        return window_;
    }

    std::shared_ptr<IScene> Window::GetScene() const {
        return scene_;
    }
}  // namespace entre_portais