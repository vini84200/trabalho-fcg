#ifndef ENTREPORTAIS_INVISIBLEBOX_HPP
#define ENTREPORTAIS_INVISIBLEBOX_HPP


#include "entrePortaisEngine/IObject.hpp"

class InvisibleBox : public entre_portais::IObject {
public:
    InvisibleBox(int x, int y);

    InvisibleBox(glm::vec3 pos, glm::vec3 scale, const char *name);

    void render(entre_portais::RenderPass current_pass) override;

    void update(float deltaTime) override;

    void initialize() override;

    void onResize(int width, int height) override;

    void onKey(int key, int scancode, int action, int mods) override;

    void onMouseButton(int button, int action, int mods) override;

    void onExit() override;
    glm::vec3 scale_;
};


#endif //ENTREPORTAIS_INVISIBLEBOX_HPP
