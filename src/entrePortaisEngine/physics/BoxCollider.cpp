#include "entrePortaisEngine/physics/BoxCollider.hpp"
#include "spdlog/spdlog.h"
#include "entrePortaisEngine/physics/SphereCollider.hpp"
#include "imgui.h"
#include "glm/gtx/string_cast.hpp"

namespace entre_portais {
    void BoxCollider::Draw() {

    }

    void BoxCollider::renderImGui(Camera *camera) {
        ImGui::Text("BoxCollider");
        ImGui::Text("Size: %s", glm::to_string(size_).c_str());
        bool showBox = ImGui::GetStateStorage()->GetBool(ImGui::GetID("showBox"));
        ImGui::Checkbox("Show Box", &showBox);
        if (showBox) {
            auto bbo = getBoundingBox();
            glm::vec3 points[8] = {
                    bbo.min,
                    {bbo.min.x, bbo.min.y, bbo.max.z},
                    {bbo.min.x, bbo.max.y, bbo.min.z},
                    {bbo.min.x, bbo.max.y, bbo.max.z},
                    {bbo.max.x, bbo.min.y, bbo.min.z},
                    {bbo.max.x, bbo.min.y, bbo.max.z},
                    {bbo.max.x, bbo.max.y, bbo.min.z},
                    bbo.max
            };
            for (int i = 0; i < 8; i++) {
                ImGui::Text("Point %d: %s", i, glm::to_string(points[i]).c_str());
            }
        }
        ImGui::GetStateStorage()->SetBool(ImGui::GetID("showBox"), showBox);
        bool showCollider = ImGui::GetStateStorage()->GetBool(ImGui::GetID("showCollider"));
        ImGui::Checkbox("Show Collider", &showCollider);
        if (showCollider) {
            // Desenha o Box Collider sobre a tela
            std::vector<glm::vec4> cubeCorners = {
                    {-size_.x / 2.0f, -size_.y / 2.0f, -size_.z / 2.0f, 1.0f},
                    {size_.x / 2.0f,  -size_.y / 2.0f, -size_.z / 2.0f, 1.0f},
                    {size_.x / 2.0f,  size_.y / 2.0f,  -size_.z / 2.0f, 1.0f},
                    {-size_.x / 2.0f, size_.y / 2.0f,  -size_.z / 2.0f, 1.0f},
                    {-size_.x / 2.0f, -size_.y / 2.0f, size_.z / 2.0f,  1.0f},
                    {size_.x / 2.0f,  -size_.y / 2.0f, size_.z / 2.0f,  1.0f},
                    {size_.x / 2.0f,  size_.y / 2.0f,  size_.z / 2.0f,  1.0f},
                    {-size_.x / 2.0f, size_.y / 2.0f,  size_.z / 2.0f,  1.0f}
            };
            for (int i = 0; i < 8; i++) {
                cubeCorners[i] = modelMatrix_ * cubeCorners[i];
            }
            for (int i = 0; i < 8; i++) {
                auto pos = camera->worldToImScreen(cubeCorners[i]);
                if (!pos.has_value()) {
                    continue;
                }
                ImGui::GetBackgroundDrawList(ImGui::GetMainViewport())->AddCircleFilled(
                        pos.value(),
                        5.0f,
                        IM_COL32(255, 255, 102, 255)
                );
                ImGui::Text("Point %d: %s", i, glm::to_string(cubeCorners[i]).c_str());
            }
            // Desenha as linhas
            for (int i = 0; i < 4; i++) {
                // Cada linha é formada por dois pontos
                // Checa se os pontos estão na tela
                camera->lineClipScreen(cubeCorners[i], cubeCorners[(i + 1) % 4], IM_COL32(255, 255, 102, 255), 2.0f);
                camera->lineClipScreen(cubeCorners[i + 4], cubeCorners[((i + 1) % 4) + 4], IM_COL32(255, 255, 102, 255),
                                       2.0f);
                camera->lineClipScreen(cubeCorners[i], cubeCorners[i + 4], IM_COL32(255, 255, 102, 255), 2.0f);
            }
        }
        ImGui::GetStateStorage()->SetBool(ImGui::GetID("showCollider"), showCollider);

    }

    BoundingBox BoxCollider::getBoundingBox(glm::mat4 transform) {
        // Cantos do cubo
        auto bbo = getBoundingBox();
        glm::vec3 points[8] = {
                bbo.min,
                {bbo.min.x, bbo.min.y, bbo.max.z},
                {bbo.min.x, bbo.max.y, bbo.min.z},
                {bbo.min.x, bbo.max.y, bbo.max.z},
                {bbo.max.x, bbo.min.y, bbo.min.z},
                {bbo.max.x, bbo.min.y, bbo.max.z},
                {bbo.max.x, bbo.max.y, bbo.min.z},
                bbo.max
        };

        // Transforma os cantos
        auto new_bbo = BoundingBox(transform * glm::vec4(points[0], 1.0f));
        for (int i = 1; i < 8; i++) {
            auto point = glm::vec3(transform * glm::vec4(points[i], 1.0f));
            new_bbo.addPoint(point);
        }

        return new_bbo;
    }

    BoxCollider::BoxCollider(glm::vec3 size, glm::mat4 &modelMatrix)
            : ICollider(modelMatrix) {
        size_ = size;
    }


    BoundingBox BoxCollider::getBoundingBox() {
        return {glm::vec3(-size_ / 2.0f), glm::vec3(size_ / 2.0f)};
    }

    collisions::PossibleCollision BoxCollider::isColliding(ICollider *other) {
        return other->isColliding(*this);
    }

    collisions::PossibleCollision BoxCollider::isColliding(glm::vec4 point) {
        return collisions::checkCollisionBoxPoint(getTransform(), point);
    }

    collisions::PossibleCollision BoxCollider::isColliding(BoxCollider box) {
        return collisions::checkCollisionBoxBox(getTransform(), box.getTransform());
    }

    collisions::PossibleCollision BoxCollider::isColliding(SphereCollider sphere) {
        return collisions::checkCollisionBoxSphere(getTransform(), sphere.getModelMatrix());
    }

    glm::mat4 BoxCollider::getTransform() {
        return modelMatrix_ * matrices::Matrix_Scale(size_.x, size_.y, size_.z);
    }

} // entre_portais