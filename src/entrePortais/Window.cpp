#include "entrePortais/Window.hpp"

#include <cstdio>
#include <cstdlib>

namespace entre_portais {
    Window::Window(int width, int height, char *title) {
        width_ = width;
        height_ = height;
        title_ = title;
        running_ = true;
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
    }

    Window::~Window() {
        glfwTerminate();
    }

    void Window::Run() {
        window_ = glfwCreateWindow(width_, height_, title_, NULL, NULL);
        if (!window_) {
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

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            fprintf(stderr, "ERROR: gladLoadGLLoader() failed.\n");
            std::exit(EXIT_FAILURE);
        }

        while (running_) {
            update();
            render();
            glfwSwapBuffers(window_);
            glfwPollEvents();
        }
        onExit();
    }

    void Window::update() {
    }

    void Window::render() {
    }

    void Window::onResize(int width, int height) {
        width_ = width;
        height_ = height;
    }

    void Window::onKey(int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            running_ = false;
        }
    }

    void Window::onMouseButton(int button, int action, int mods) {
    }

    void Window::onError(int error, const char *description) {
        fprintf(stderr, "ERROR: %s\n", description);
    }

    void Window::onExit() {
        printf("Bye bye!\n");
    }
}  // namespace entre_portais