#include "testGame/TriObject.hpp"
#include "entrePortaisEngine/meshes/EasyMesh.hpp"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "testGame/DependencyTask.hpp"
#include "entrePortaisEngine/tasks/TaskHandler.hpp"
#include "entrePortaisEngine/tasks/LambdaTask.hpp"
#include "entrePortaisEngine/Compatibility.hpp"
#include "entrePortaisEngine/IScene.hpp"

entre_portais::TriObject::TriObject(char *name) : IObject(name), logger_(name) {
    auto vert = new entre_portais::ManyVertices();
    vert->vertices.push_back({-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f});
    vert->vertices.push_back({0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f});
    vert->vertices.push_back({0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f});
    vert->indices.push_back(0);
    vert->indices.push_back(1);
    vert->indices.push_back(2);
    mesh_ = std::make_shared<EasyMesh>(*vert, "tri");

    delete vert;
    transform_.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    transform_.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    transform_.setRotationZYX(glm::vec3(0.0f, 0.0f, 0.0f));
}

void entre_portais::TriObject::update(float deltaTime) {
    // Gira o triângulo
    transform_.rotateBy(velocity_ * deltaTime, 0.0f, 0.0f);
}

void entre_portais::TriObject::initialize() {
    auto renderer = IObject::getScene()->getRenderer();
    loadShader("tri");
    submit(renderer, RenderPass::FOREGROUND);
    logger_.getLogger()->info("Submetendo para renderizacao TriObject");
}

void entre_portais::TriObject::onResize(int width, int height) {

}

void entre_portais::TriObject::onExit() {

}

void entre_portais::TriObject::onMouseButton(int button, int action, int mods) {

}

void entre_portais::TriObject::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
        auto tm = entre_portais::TaskManager::getInstance();
        auto a = tm->addTask<DependencyTask>();

        tm->addTask<LambdaTask<EmptyState>>([](EmptyState) -> TaskRunResult {
            sleep_for_millis(2000);
            return TaskRunResult::SUCCESS;
        }, std::string("Simple Lambda Task"));

        struct MyState {
            int value = 0;

            MyState() {
                value = 0;
            };
        };

        auto lambdaTaskWithState = tm->addTask<LambdaTask<MyState>>([](MyState &state) -> TaskRunResult {
            if (state.value >= 1) {
                sleep_for_millis(2000);
                return TaskRunResult::SUCCESS;
            }
            state.value++;
            spdlog::info("Value is {}", state.value);
            sleep_for_millis(1000);
            return TaskRunResult::REPEAT;
        }, std::string("Lambda Task With State"));

        tm->addTask<LambdaTask<MyState>>([=](MyState &state) -> TaskRunResult {
            if (lambdaTaskWithState.getStatus() == TaskStatus::SUCCESS) {
                return TaskRunResult::SUCCESS;
            } else {
                return TaskRunResult::REPEAT;
            }
        }, std::string("Lambda Task With Reference"));
    }
}

void entre_portais::TriObject::CustomImGui() {
    ImGui::SliderFloat("Velocity", &velocity_, 0.0f, 200.0f);
}
