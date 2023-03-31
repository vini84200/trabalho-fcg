#include "entrePortaisEngine/gui/GuiRectangle.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/render/TextureManager.hpp"
#include "glm/gtx/string_cast.hpp"
#include "GLFW/glfw3.h"

namespace entre_portais {
    void GuiRectangle::update(float deltaTime) {
    }

    void GuiRectangle::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        registerGuiObject(renderer, "quatUi");
    }

    void GuiRectangle::renderUi(VertexArrayBuffer *quadVao, Shader shader) {
        setupUniformsForGui(shader);
        shader.setUniformVec4("color", color);
        shader.setUniformUInt("flags",
                              1 * (isUsingTexture() ? 1 : 0)
                              + 2 * (isUsingHoverTexture() ? 1 : 0));
        if (isUsingTexture()) {
            shader.setUniformInt("texture", 0);
            TextureManager::instance().getTexture(texture_path_).Bind(0);
        }
        if (isUsingHoverTexture()) {
            shader.setUniformInt("textureHover", 1);
            TextureManager::instance().getTexture(hover_texture_path_).Bind(1);
        }
        shader.setUniformInt("isHovering", isInHoverState());
        drawQuad(quadVao);
    }

    GuiRectangle::GuiRectangle(const char *name, std::string texture_path) : GuiObject(name),
                                                                             texture_path_(texture_path) {
        setName(name);
        texture_path_ = texture_path;
        // Initialize the texture
        TextureManager::instance().LoadTexture(texture_path_);

    }

    void GuiRectangle::loadTextures() {
        if (isUsingTexture()) {
            TextureManager::instance().LoadTexture(texture_path_);
        }
        if (isUsingHoverTexture()) {
            TextureManager::instance().LoadTexture(hover_texture_path_);
        }
    }

    void GuiRectangle::setHoverTexturePath(std::string texture_path) {
        hover_texture_path_ = texture_path;
        loadTextures();
    }

    void GuiRectangle::setTexturePath(std::string texture_path) {
        texture_path_ = texture_path;
        loadTextures();
    }

    void GuiRectangle::CustomImGui() {
        ImGui::Text("GuiRectangle");
        ImGui::Text("Color: %s", glm::to_string(color).c_str());
        ImGui::Text("Texture: %s", texture_path_.c_str());
        ImGui::Text("Hover Texture: %s", hover_texture_path_.c_str());
        ImGui::Text("Hovering: %s", isInHoverState() ? "true" : "false");
        ImGui::Text("Bounds: %s - %s", glm::to_string(getBBMinScreenSpace()).c_str(),
                    glm::to_string(getBBMaxScreenSpace()).c_str());
        double x, y;
        glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
        ImGui::Text("Mouse: %f, %f", x, y);
    }
} // entre_portais