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

    void GuiRectangle::renderUi(VertexArrayBuffer *quadVao, Shader &shader) {
        setupUniformsForGui(shader);

        shader.setUniformVec4("color", color);
        shader.setUniformUInt("flags",
                              1 * (isUsingTexture() ? 1 : 0)
                              + 2 * (isUsingHoverTexture() ? 1 : 0));
        if (isUsingTexture()) {
            shader.setUniformInt("textureBase", 0);
            TextureManager::instance().getTextureSync(texture_path_).Bind(0);
        }
        if (isUsingHoverTexture()) {
            shader.setUniformInt("textureHover", 1);
            TextureManager::instance().getTextureSync(hover_texture_path_).Bind(1);
        }
        shader.setUniformInt("isHovering", isInHoverState());
        shader.setUniformVec2("textureResize", texture_size_);
        shader.setUniformVec2("textureOffset", texture_offset_);

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

    void GuiRectangle::onClick(float posX, float posY) {
        spdlog::trace("GuiRectangle::onClick({},{})", posX, posY);
        if (click_callback_) {
            click_callback_(posX, posY);
        }
    }

    void GuiRectangle::registerClickCallback(std::function<void(float, float)> callback) {
        click_callback_ = callback;
    }

    const glm::vec2 &GuiRectangle::getTextureSize() const {
        return texture_size_;
    }

    void GuiRectangle::setTextureSize(const glm::vec2 &textureSize) {
        texture_size_ = textureSize;
    }

    const glm::vec2 &GuiRectangle::getTextureOffset() const {
        return texture_offset_;
    }

    void GuiRectangle::setTextureOffset(const glm::vec2 &textureOffset) {
        texture_offset_ = textureOffset;
    }
} // entre_portais