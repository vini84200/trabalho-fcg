#include "entrePortaisEngine/gui/GuiRectangle.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "glm/gtx/string_cast.hpp"

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
        drawQuad(quadVao);
    }
} // entre_portais