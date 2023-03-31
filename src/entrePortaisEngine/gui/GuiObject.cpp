#include "entrePortaisEngine/gui/GuiObject.hpp"
#include "GLFW/glfw3.h"

namespace entre_portais {

    void GuiObject::render(RenderPass current_pass) {
        // Do nothing
    }

    void GuiObject::onResize(int width, int height) {
        currentWindowWidth_ = static_cast<float>(width);
        currentWindowHeight_ = static_cast<float>(height);
        // Use the layout type to calculate the new position
        recalculatePosition();
    }

    void GuiObject::recalculatePosition() {
        float screen_width = currentWindowWidth_;
        float screen_height = currentWindowHeight_;
        currentPosX_ = std::visit(PositionXConstraintsVisit(screen_width, screen_height), x_);
        currentPosY_ = std::visit(PositionYConstraintsVisit(screen_width, screen_height), y_);
        currentWidth_ = std::visit(ScaleXConstraintsVisit(screen_width, screen_height), width_);
        currentHeight_ = std::visit(ScaleYConstraintsVisit(screen_width, screen_height), height_);
    }

    GuiObject::~GuiObject() {
        // Nothing to do for now
    }

    void GuiObject::onKey(int key, int scancode, int action, int mods) {

    }

    void GuiObject::onMouseButton(int button, int action, int mods) {

    }

    void GuiObject::onExit() {

    }

    zIndex_t GuiObject::getZIndex() const {
        return zIndex_;
    }

    void GuiObject::setZIndex(zIndex_t zIndex) {
        zIndex_ = zIndex;
    }

    const PositionConstraint &GuiObject::getXConstraint() const {
        return x_;
    }

    void GuiObject::setX(const PositionConstraint &x) {
        x_ = x;
        recalculatePosition();
    }

    const PositionConstraint &GuiObject::getYConstraint() const {
        return y_;
    }

    void GuiObject::setY(const PositionConstraint &y) {
        y_ = y;
        recalculatePosition();
    }

    void GuiObject::setWitdhtConstraint(const ScaleConstraint &width) {
        width_ = width;
        recalculatePosition();
    }

    void GuiObject::setHeightConstraint(const ScaleConstraint &height) {
        height_ = height;
        recalculatePosition();
    }

    float GuiObject::getPositionX() const {
        return currentPosX_;
    }

    float GuiObject::getPositionY() const {
        return currentPosY_;
    }

    float GuiObject::getWidth() const {
        return currentWidth_;
    }

    float GuiObject::getHeight() const {
        return currentHeight_;
    }

    GuiObject::GuiObject(const char *name)
            : IObject(name), x_(FixedPosition(0.0f)), y_(FixedPosition(0.0f)), width_(RelativeScale(1.0f)),
              height_(RelativeScale(1.0f)), zIndex_(0) {
        int width, height;
        glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
        onResize(width, height);
    }

    GuiObject::GuiObject(const char *name, PositionConstraint x, PositionConstraint y, ScaleConstraint width,
                         ScaleConstraint height, zIndex_t zIndex)
            : IObject(name), x_(x), y_(y), width_(width), height_(height), zIndex_(zIndex) {
        int w, h;
        glfwGetWindowSize(glfwGetCurrentContext(), &w, &h);
        onResize(w, h);

    }

    void GuiObject::setupUniformsForGui(Shader shader) {
        shader.setUniformVec2("screen_size", glm::vec2(currentWindowWidth_, currentWindowHeight_));

        shader.setUniformVec2("position", getPosToNDC());
        shader.setUniformVec2("size", getSizeToNDC());
        shader.setUniformFloat("z_index", static_cast<float>(zIndex_));
        // TODO: mouse position
        // TODO: mouse click
        // TODO: time

    }

    void GuiObject::registerGuiObject(std::shared_ptr<Renderer> renderer, std::string shader_name) {
        loadShader(shader_name);
        submit(renderer, entre_portais::RenderPass::UI);
    }

    void GuiObject::drawQuad(VertexArrayBuffer *quadVao) {
        quadVao->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        quadVao->unbind();
    }

    bool GuiObject::isInHoverState() const {
        double currentMousePosX, currentMousePosY;
        glfwGetCursorPos(glfwGetCurrentContext(), &currentMousePosX, &currentMousePosY);
        return isInsideBB(static_cast<float>(currentMousePosX), static_cast<float>(currentMousePosY));
    }


} // entre_portais