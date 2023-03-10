#ifndef ENTREPORTAIS_CAMERA_HPP
#define ENTREPORTAIS_CAMERA_HPP

#include "entrePortaisEngine/IGameNode.hpp"
#include <memory>
#include <optional>
#include "ShadersManager.hpp"
#include "entrePortaisEngine/Logger.hpp"
#include "imgui.h"

namespace entre_portais {

    class Camera : public IGameNode {
    public:
        Camera(char *name, float fov, float near, float far, float aspectRatio);

        ~Camera() override {};

        Camera(const Camera &other) = delete;

        Camera &operator=(const Camera &other) = delete;

        Camera(Camera &&other) = delete;

        Camera &operator=(Camera &&other) = delete;

        void renderImGui(bool *p_open) override;

        void setUpCamera();

        void configureShaderUniforms(Shader shader);

        glm::mat4 getProjectionMatrix();

        glm::mat4 getViewMatrix();

        void update(float deltaTime) override {};

        void initialize() override {};

        void onResize(int width, int height) override;

        void onKey(int key, int scancode, int action, int mods) override {};

        void onMouseButton(int button, int action, int mods) override {};

        void onExit() override {};

        void CustomImGui() override {};

        std::shared_ptr<IScene> getScene() override;

        void setScene(std::shared_ptr<IScene> scene) override;

        bool hasScene() override;

        glm::vec4 getViewVector();

        std::optional<ImVec2> worldToImScreen(glm::vec4 worldPoint);

        std::optional<ImVec2> worldToImScreen(glm::vec3 worldPoint);

        void lineClipScreen(glm::vec2 &point1, glm::vec2 &point2, ImU32 color, float thickness);

        void lineClipScreen(glm::vec3 &point1, glm::vec3 &point2, ImU32 color, float thickness);

        void lineClipScreen(glm::vec4 &point1, glm::vec4 &point2, ImU32 color, float thickness);

        bool isPointInsideNDC(glm::vec4 point);


        void debugDrawCube(const glm::vec3 center, const glm::vec3 size, ImU32 color);

        void debugDrawLine(glm::vec3 point1, glm::vec3 point2, ImU32 color, float thickness);

        void debugDrawPoint(glm::vec4 global_pos, const ImU32 color, float size);

    private:
        std::weak_ptr<IScene> scene_;
        float far_;
        float near_;
        float fov_;
        float aspectRatio_;
        int width_;
        int height_;
    };

}  // namespace entre_Portais

#endif  // ENTREPORTAIS_CAMERA_HPP
