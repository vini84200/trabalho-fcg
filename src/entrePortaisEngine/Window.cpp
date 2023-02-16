#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/Logger.hpp"
#include "utils/utils.hpp"
#include <unistd.h>

namespace entre_portais {
    Window::Window(int width, int height, const char *title, std::shared_ptr<IScene> scene) {

        if (scene == NULL) {
            throw std::invalid_argument("Scene cannot be null");
        }
        width_ = width;
        height_ = height;
        title_ = (char *) title;
        running_ = true;
        scene_ = scene;
        int success = glfwInit();
        if (!success) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwSetErrorCallback(onError);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window_ = glfwCreateWindow(width_, height_, title_, NULL, NULL);

        if (!window_) {
            getLogger()->error("Failed to create GLFW window");
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
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
            getLogger()->error("Failed to initialize GLAD");
            throw std::runtime_error("Failed to initialize GLAD");
        }

        char *vendor = (char *) glGetString(GL_VENDOR);
        char *renderer = (char *) glGetString(GL_RENDERER);
        char *glversion = (char *) glGetString(GL_VERSION);
        char *glslversion = (char *) glGetString(GL_SHADING_LANGUAGE_VERSION);

        getLogger()->info("OpenGL Vendor: {} Renderer: {} Version: {} GLSL Version: {}", vendor, renderer, glversion,
                          glslversion);

        glfwSwapInterval(VSYNC);
        scene_->initialize();
    }

    Window::~Window() {
        getLogger()->info("Destroying window");
        scene_.reset();
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    void Window::Run() {
        lastFrameTime_ = glfwGetTime() - 1.0 / targetFPS_;
        getLogger()->info("Iniciando o loop principal");
        getLogger()->info("Target FPS: {} / Target Frame Time: {}", targetFPS_, (1.0f / targetFPS_));
        while (running_) {
            // Calculate delta time
            double currentFrame = glfwGetTime();
            double timeDifference = currentFrame - lastFrameTime_;
            lastFrameTime_ = currentFrame;

            // Update
            // Usa Semi-fixed timestep (veja https://gafferongames.com/post/fix_your_timestep/)
            try {
                while (timeDifference > 0.0f) {
                    float deltaTime = utils::min(timeDifference, 1.0 / TARGET_UPS);
                    update(deltaTime);
                    timeDifference -= deltaTime;
                }
            }
            catch (const std::exception &e) {
                getLogger()->error("Exception while updating: {}", e.what());
                spdlog::dump_backtrace();
                std::rethrow_exception(std::current_exception());
            } catch (...) {
                getLogger()->error("Unknown exception while updating");
                spdlog::dump_backtrace();
                std::rethrow_exception(std::current_exception());
            }

            // Render
            try {
                render();
            }
            catch (const std::exception &e) {
                getLogger()->error("Exception while rendering: {}", e.what());
                spdlog::dump_backtrace();
                std::rethrow_exception(std::current_exception());
            } catch (...) {
                getLogger()->error("Unknown exception while rendering");
                spdlog::dump_backtrace();
                std::rethrow_exception(std::current_exception());
            }

            glfwSwapBuffers(window_);
            glfwPollEvents();
            if (timeDifference < 1.0 / targetFPS_) {
#ifdef USE_OPSYS_SLEEP
                entre_portais::sleep_for_nanosecods((1.0 / targetFPS_ - deltaTime) * 1000000);
#endif
            } else if (timeDifference > 1.0 / WARNING_FPS) {
                getLogger()->warn("Running behind by {} seconds, expected {} seconds", timeDifference,
                                  1.0 / targetFPS_);
            }

        }
        scene_->onExit();
        onExit();
    }

    void Window::update(float deltaTime) {
        for (auto &plugin: registeredPlugins_) {
            plugin->update(deltaTime);
        }
        scene_->updatePropagate(deltaTime);
    }

    void Window::render() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        scene_->renderPropagate();
        for (auto &plugin: registeredPlugins_) {
            plugin->render();
        }
    }

    void Window::onResize(int width, int height) {
        width_ = width;
        height_ = height;
//        getLogger()->info("Window resized to {} x {}", width_, height_);
        scene_->resize(width_, height_);
    }

    void Window::onKey(int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_Q && action == GLFW_PRESS && mods == GLFW_MOD_CONTROL) {
            running_ = false;
        }

        getLogger()->trace("Key {} {}", key, action == GLFW_PRESS ? "pressed" : "released");

        for (auto plugin: registeredPlugins_) {
            plugin->onKey(key, scancode, action, mods);
        }

        try {
            scene_->keyPress(key, scancode, action, mods);
        } catch (const std::exception &e) {
            getLogger()->error("Exception while handling key press: {}", e.what());
            spdlog::dump_backtrace();
            std::rethrow_exception(std::current_exception());
        } catch (...) {
            getLogger()->error("Unknown exception while handling key press");
            spdlog::dump_backtrace();
            std::rethrow_exception(std::current_exception());
        }
    }

    void Window::onMouseButton(int button, int action, int mods) {
        getLogger()->info("Mouse button {} {}", button, action == GLFW_PRESS ? "pressed" : "released");
        scene_->mouseButton(button, action, mods);
    }

    void Window::onError(int /*error*/, const char *description) {
        spdlog::error("GLFW error: {}", description);
        throw std::runtime_error(description);
    }

    void Window::onCloseWindow() {
        running_ = false;
    }

    void Window::onExit() {
        getLogger()->info("Exiting");
        scene_->exit();
        UnregisterAllPlugins();
    }

    void Window::RegisterPlugin(std::shared_ptr<IPlugin> plugin) {
        registeredPlugins_.push_back(plugin);
        plugin->SetWindow(shared_from_this());
        plugin->onAttach();
    }

    void Window::UnregisterPlugin(std::shared_ptr<IPlugin> plugin) {
        plugin->onDetach();
        plugin->SetWindow(nullptr);
        registeredPlugins_.erase(std::find(registeredPlugins_.begin(), registeredPlugins_.end(), plugin));
        getLogger()->info("Unregistered plugin");
    }

    void Window::UnregisterAllPlugins() {
        getLogger()->info("Unregistering all plugins");
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