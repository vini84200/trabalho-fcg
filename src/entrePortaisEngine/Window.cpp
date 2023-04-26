#include "entrePortaisEngine/Window.hpp"
#include "entrePortaisEngine/Compatibility.hpp"
#include "entrePortaisEngine/Logger.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "utils/utils.hpp"
#include <bits/types/struct_tm.h>
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
        mousePos_.x = width_ / 2;
        mousePos_.y = height_ / 2;
        int success = glfwInit();
        if (!success) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwSetErrorCallback(onError);

#ifdef OLD_OPENGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window_ = glfwCreateWindow(width_, height_, title_, NULL, NULL);

        if (!window_) {
            getLogger()->error("Failed to create GLFW window");
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window_);


        cursorIsVisible_ = true;


        glfwSetWindowUserPointer(window_, this);
        glfwSetWindowSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
            Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
            self->onResize(width, height);
        });
        glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
            self->onKey(key, scancode, action, mods);
        });
        glfwSetCursorPosCallback(window_, [](GLFWwindow *window, double xpos, double ypos) {
            Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
            self->onMouseMovement(xpos, ypos);
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
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        scene_->setWindow(this);
        scene_->initializePropagate();
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
        float accumulator = 0.0f;
        while (running_) {
            // Calculate delta time
            double currentFrame = glfwGetTime();
            double timeDifference = currentFrame - lastFrameTime_;
            if (timeDifference > 1.0 / WARNING_FPS) {
                getLogger()->warn("Running behind by {} seconds, expected {} seconds", timeDifference,
                                  1.0 / targetFPS_);
                timeDifference = 1.0 / targetFPS_;
            }
            lastFrameTime_ = currentFrame;

            accumulator += timeDifference;
            float deltaTime = 1.0 / TARGET_UPS;

            // If there is a need to change the scene, do it
            if (nextScene_ != nullptr) {
                scene_->onExit();
                scene_.swap(nextScene_);
                scene_->setWindow(this);
                scene_->initializePropagate();
                nextScene_.reset();
            }

            // Update
            // Usa fixed timestep (veja https://gafferongames.com/post/fix_your_timestep/)
            try {
                while (accumulator >= deltaTime) {
                    update(deltaTime);
                    accumulator -= deltaTime;
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

            // render
            try {
                render();
                frameCount_++;
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

            // Run some sync tasks
            auto tm = TaskManager::getInstance();
            float maxTime = 1.0 / targetFPS_ - timeDifference;
            tm->RunSyncTasks(maxTime);

            if (timeDifference < 1.0 / targetFPS_) {
            } else if (timeDifference > 1.0 / WARNING_FPS) {
                getLogger()->warn("Running behind by {} seconds, expected {} seconds", timeDifference,
                                  1.0 / targetFPS_);
            }

        }
        scene_->onExit();
        onExit();
    }

    long Window::getFrameCount() const {
        return frameCount_;
    }

    void Window::update(float deltaTime) {
        for (auto &plugin: registeredPlugins_) {
            plugin->update(deltaTime);
        }
        scene_->updatePropagate(deltaTime);
    }

    void Window::render() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene_->preRenderPropagate();
        scene_->render();
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

//        getLogger()->trace("Key {} {}", key, action == GLFW_PRESS ? "pressed" : "released");
        bool handled = false;
        for (auto plugin: registeredPlugins_) {
            handled = plugin->onKey(key, scancode, action, mods);
            if (handled) {
                return;
            }
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
//        getLogger()->info("Mouse button {} {}", button, action == GLFW_PRESS ? "pressed" : "released");
        bool handled = false;
        for (auto plugin: registeredPlugins_) {
            handled = plugin->onMouseButton(button, action, mods);
            if (handled) {
                return;
            }
        }
        scene_->mouseButton(button, action, mods);
    }

    void Window::onMouseMovement(double xpos, double ypos) {
        bool handled = false;
        for (auto plugin: registeredPlugins_) {
            handled = plugin->onMouseMove(xpos, ypos);
            if (handled) {
                return;
            }
        }
        scene_->mouseMovementPropagate(xpos, ypos);
        glm::vec2 mousePos(xpos, ypos);
        glm::vec2 delta = mousePos - mousePos_;
        mousePos_ = mousePos;
        onMouseDeltaMovement(delta);
    }

    void Window::onMouseDeltaMovement(glm::vec2 delta) {
        scene_->mouseDeltaMovementPropagate(delta);
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
        TaskManager::getInstance()->stop();
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

    void Window::showCursor(bool show) {
        if (show) {
            cursorIsVisible_ = true;
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            cursorIsVisible_ = false;
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    bool Window::isCursorVisible() {
        return cursorIsVisible_;
    }

    void Window::setRunning(bool running) {
        running_ = running;
    }


}  // namespace entre_portais