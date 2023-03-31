#ifndef ENTREPORTAIS_GUIRECTANGLE_HPP
#define ENTREPORTAIS_GUIRECTANGLE_HPP

#include "GuiObject.hpp"

namespace entre_portais {

    class GuiRectangle : public GuiObject {
    public:
        GuiRectangle(const char *name, glm::vec4 color) : GuiObject(name), color(color) {}

        void update(float deltaTime) override;

        void initialize() override;

        void renderUi(VertexArrayBuffer *quadVao, Shader shader) override;

    private:
        glm::vec4 color;
    };

} // entre_portais

#endif //ENTREPORTAIS_GUIRECTANGLE_HPP
