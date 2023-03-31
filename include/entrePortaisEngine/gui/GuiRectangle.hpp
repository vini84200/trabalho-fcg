#ifndef ENTREPORTAIS_GUIRECTANGLE_HPP
#define ENTREPORTAIS_GUIRECTANGLE_HPP

#include "GuiObject.hpp"

namespace entre_portais {

    class GuiRectangle : public GuiObject {
    public:
        GuiRectangle(const char *name, glm::vec4 color) : GuiObject(name), color(color), texture_path_("") {};

        GuiRectangle(const char *name, std::string texture_path);

        void update(float deltaTime) override;

        void initialize() override;

        void renderUi(VertexArrayBuffer *quadVao, Shader shader) override;

        void setColor(glm::vec4 color) { this->color = color; }

        void setTexturePath(std::string texture_path);

        void setNoTexture() { this->texture_path_ = ""; }

        bool isUsingTexture() const { return !texture_path_.empty(); }

        std::string getTexturePath() const { return texture_path_; }

        void setHoverTexturePath(std::string texture_path);

        void setNoHoverTexture() { hover_texture_path_ = ""; }

        bool isUsingHoverTexture() const { return !hover_texture_path_.empty(); }

        std::string getHoverTexturePath() const { return hover_texture_path_; }

        const glm::vec2 &getTextureSize() const;

        void setTextureSize(const glm::vec2 &textureSize);

        const glm::vec2 &getTextureOffset() const;

        void setTextureOffset(const glm::vec2 &textureOffset);

        void loadTextures();

        void CustomImGui() override;

        void onClick(float posX, float posY) override;

        void registerClickCallback(std::function<void(float, float)> callback);


    private:
        glm::vec4 color;
        std::string texture_path_;
        std::string hover_texture_path_;
        std::function<void(float, float)> click_callback_;
        glm::vec2 texture_size_ = glm::vec2(1.0f, 1.0f);
        glm::vec2 texture_offset_ = glm::vec2(0.0f, 0.0f);
    };

} // entre_portais

#endif //ENTREPORTAIS_GUIRECTANGLE_HPP
