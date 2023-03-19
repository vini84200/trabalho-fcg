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
        width_ = width;
        height_ = height;
    }

    glm::mat4 Camera::getViewMatrix() {
        return glm::inverse(modelMatrix_);
    }

    glm::vec4 Camera::getViewVector() {
        return normalize(glm::vec4(0, 0, -1, 0) * transform_.getRotation());
    }

    std::optional<ImVec2> Camera::worldToImScreen(glm::vec4 worldPoint) {
        glm::mat4 viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
        glm::vec4 screenPoint = viewProjectionMatrix * worldPoint;
        if (screenPoint.w <= 0) {
            return std::nullopt;
        }
        screenPoint /= screenPoint.w;
        // Remove the rotation from the world point
//        screenPoint = glm::inverse(transform_.getRotation()) * screenPoint;

        screenPoint.x = (screenPoint.x + 1.0f) / 2.0f;
        screenPoint.y *= -1;
        screenPoint.y = (screenPoint.y + 1.0f) / 2.0f;
        screenPoint.z = (screenPoint.z + 1.0f) / 2.0f;

        ImVec2 s = ImGui::GetMainViewport()->Size;
        ImVec2 p = ImGui::GetMainViewport()->Pos;

        screenPoint.x *= s.x;
        screenPoint.y *= s.y;

        screenPoint.x += p.x;
        screenPoint.y += p.y;

        return ImVec2(screenPoint.x, screenPoint.y);
    }


    void Camera::lineClipScreen(glm::vec2 &point1, glm::vec2 &point2, ImU32 color, float thickness) {
        // Line clipping algorithm
        // FONTE: https://stackoverflow.com/questions/11194876/clip-line-to-screen-coordinates
        float t0 = 0.0;
        float t1 = 1.0;
        float xdelta = point2.x - point1.x;
        float ydelta = point2.y - point1.y;
        float p, q, r;
        const auto size = ImGui::GetMainViewport()->Size;
        const auto pos = ImGui::GetMainViewport()->Pos;
        const auto edgeLeft = pos.x;
        const auto edgeRight = pos.x + size.x;
        const auto edgeBottom = pos.y;
        const auto edgeTop = pos.y + size.y;


        for (int edge = 0; edge < 4; edge++) {   // Traverse through left, right, bottom, top edges.
            if (edge == 0) {
                p = -xdelta;
                q = -(edgeLeft - point1.x);
            }
            if (edge == 1) {
                p = xdelta;
                q = (edgeRight - point1.x);
            }
            if (edge == 2) {
                p = -ydelta;
                q = -(edgeBottom - point1.y);
            }
            if (edge == 3) {
                p = ydelta;
                q = (edgeTop - point1.y);
            }
            r = q / p;
            if (p == 0 && q < 0) return;   // Don't draw line at all. (parallel line outside)

            if (p < 0) {
                if (r > t1) return;         // Don't draw line at all.
                else if (r > t0) t0 = r;            // Line is clipped!
            } else if (p > 0) {
                if (r < t0) return;      // Don't draw line at all.
                else if (r < t1) t1 = r;         // Line is clipped!
            }
        }

        float x0clip = point1.x + t0 * xdelta;
        float y0clip = point1.y + t0 * ydelta;
        float x1clip = point1.x + t1 * xdelta;
        float y1clip = point1.y + t1 * ydelta;

        ImGui::GetBackgroundDrawList(ImGui::GetMainViewport())->AddLine(
                ImVec2(x0clip, y0clip),
                ImVec2(x1clip, y1clip),
                color,
                thickness
        );

    }

    std::optional<ImVec2> Camera::worldToImScreen(glm::vec3 worldPoint) {
        return worldToImScreen(glm::vec4(worldPoint, 1.0f));
    }

    bool Camera::isPointInsideNDC(glm::vec4 point) {
        glm::mat4 viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
        point = viewProjectionMatrix * point;
        point /= point.w;
        point.y *= -1;
        return point.x >= -1 && point.x <= 1 && point.y >= -1 && point.y <= 1;
    }

    void Camera::lineClipScreen(glm::vec3 &point1, glm::vec3 &point2, ImU32 color, float thickness) {
        auto p1_ = worldToImScreen(point1);
        auto p2_ = worldToImScreen(point2);
        if (!p1_.has_value() || !p2_.has_value()) {
            return;
        }
        auto p1 = p1_.value();
        auto p2 = p2_.value();
        glm::vec2 p1__ = glm::vec2(p1.x, p1.y);
        glm::vec2 p2__ = glm::vec2(p2.x, p2.y);

        lineClipScreen(p1__, p2__, color, thickness);
    }

    void Camera::lineClipScreen(glm::vec4 &point1, glm::vec4 &point2, ImU32 color, float thickness) {
        auto p1_ = worldToImScreen(point1);
        auto p2_ = worldToImScreen(point2);
        if (!p1_.has_value() || !p2_.has_value()) {
            return;
        }
        auto p1 = p1_.value();
        auto p2 = p2_.value();
        glm::vec2 p1__ = glm::vec2(p1.x, p1.y);
        glm::vec2 p2__ = glm::vec2(p2.x, p2.y);

        lineClipScreen(p1__, p2__, color, thickness);
    }

    void Camera::debugDrawCube(const glm::vec3 center, const glm::vec3 size, ImU32 color) {

        std::vector<glm::vec4> cubeCorners = {
                {center.x - size.x / 2, center.y - size.y / 2, center.z - size.z / 2, 1},
                {center.x + size.x / 2, center.y - size.y / 2, center.z - size.z / 2, 1},
                {center.x + size.x / 2, center.y + size.y / 2, center.z - size.z / 2, 1},
                {center.x - size.x / 2, center.y + size.y / 2, center.z - size.z / 2, 1},
                {center.x - size.x / 2, center.y - size.y / 2, center.z + size.z / 2, 1},
                {center.x + size.x / 2, center.y - size.y / 2, center.z + size.z / 2, 1},
                {center.x + size.x / 2, center.y + size.y / 2, center.z + size.z / 2, 1},
                {center.x - size.x / 2, center.y + size.y / 2, center.z + size.z / 2, 1},
        };

        // Use imGui to draw lines
        for (int i = 0; i < 4; i++) {
            lineClipScreen(cubeCorners[i], cubeCorners[(i + 1) % 4], color, 1.0f);
            lineClipScreen(cubeCorners[i + 4], cubeCorners[((i + 1) % 4) + 4], color, 1.0f);
            lineClipScreen(cubeCorners[i], cubeCorners[i + 4], color, 1.0f);
        }
    }

    void Camera::debugDrawLine(glm::vec3 point1, glm::vec3 point2, ImU32 color, float thickness) {
        lineClipScreen(point1, point2, color, thickness);
    }

    void Camera::debugDrawPoint(glm::vec4 global_pos, const ImU32 color, float size) {
        auto screen_pos = worldToImScreen(global_pos);
        if (!screen_pos.has_value()) {
            return;
        }
        ImGui::GetBackgroundDrawList(ImGui::GetMainViewport())->AddCircleFilled(screen_pos.value(), size, color);
    }

}