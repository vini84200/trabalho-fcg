#include "entrePortaisEngine/render/Camera.hpp"

#include "entrePortaisEngine/IScene.hpp"
#include "utils/matrices.h"
#include "glm/gtx/string_cast.hpp"

namespace entre_portais {

    void Camera::renderImGui(bool *p_open) {
        if (ImGui::TreeNode(getName())) {
            transform_.renderImGui();
            if (ImGui::TreeNode("Camera Options")) {
                ImGui::DragFloat("Far", &far_);
                ImGui::DragFloat("Near", &near_);
                ImGui::DragFloat("Aspect Ratio", &aspectRatio_);
                ImGui::DragFloat("Field of View", &fov_);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Camera Log")) {

                glm::vec4 viewVector = getViewVector();
                ImGui::Text("View Vector: %s", glm::to_string(viewVector).c_str());

                glm::mat4 viewMatrix = getViewMatrix();
                ImGui::Text("View Matrix:\n");
                ImGui::Text(" %s", glm::to_string(viewMatrix).c_str());

                glm::mat4 projectionMatrix = getProjectionMatrix();
                ImGui::Text("Projection Matrix:\n");
                ImGui::Text(" %s", glm::to_string(projectionMatrix).c_str());

                glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
                ImGui::Text("View Projection Matrix:\n");
                ImGui::Text(" %s", glm::to_string(viewProjectionMatrix).c_str());

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Children")) {
                for (auto &child: children_) {
                    child->renderImGui();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }

    Camera::Camera(char *name, float fov, float n, float f, float aspectRatio) : IGameNode(name) {
        fov_ = fov;
        near_ = n;
        far_ = f;
        aspectRatio_ = aspectRatio; // FIXME: This should be initialized by the window size
    }

    std::shared_ptr<entre_portais::IScene> entre_portais::Camera::getScene() {
        if (scene_.expired()) {
            throw std::runtime_error("A cena não existe mais.");
        }
        return scene_.lock();
    }

    void entre_portais::Camera::setScene(std::shared_ptr<IScene> scene) {
        scene_ = scene;
    }

    void Camera::setUpCamera() {
        // TODO
    }

    void Camera::configureShaderUniforms(Shader shader) {
        shader.setUniformMat4("view", getViewMatrix());
        shader.setUniformMat4("projection", getProjectionMatrix());
    }

    bool entre_portais::Camera::hasScene() {
        return !scene_.expired();
    }

    glm::mat4 Camera::getProjectionMatrix() {
        return matrices::Matrix_Perspective(fov_, aspectRatio_, -near_, -far_);
    }

    void Camera::onResize(int width, int height) {
        glViewport(0, 0, width, height);
        aspectRatio_ = ((float) width) / ((float) height);
    }

    glm::mat4 Camera::getViewMatrix() {
//        auto model = glm::inverse(modelMatrix_);
//        return model * matrices::Matrix_Camera_View(glm::vec4(0, 0, 0, 1), getViewVector(), glm::vec4(0, 1, 0, 0));
        return glm::inverse(modelMatrix_);
    }

    glm::vec4 Camera::getViewVector() {
        return normalize(glm::vec4(0, 0, -1, 0) * transform_.getRotation());
    }

    void Camera::preRender() {
        IGameNode::preRender();
        auto parentModelMatrix = getParentModelMatrix();
        viewMatrix_ = glm::inverse(*parentModelMatrix) *
                      matrices::Matrix_Camera_View(transform_.getPosition(), getViewVector(), glm::vec4(0, 1, 0, 0));
    }

}