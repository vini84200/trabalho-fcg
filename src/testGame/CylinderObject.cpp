#include "testGame/CylinderObject.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/meshes/CylinderMesh.hpp"
#include "entrePortaisEngine/meshes/PlaneMesh.hpp"
#include "GLFW/glfw3.h"

namespace entre_portais {
    void CylinderObject::CustomImGui() {

    }

    void CylinderObject::update(float deltaTime) {
        if (moving_) {
            float t = t_;
            t_ += deltaTime / 3;
            if (t_ > 3) {
                t_ -= deltaTime;
                moving_ = false;
            }
            bezierMove(t);
        }
    }

    void CylinderObject::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("primitive");
        submit(renderer);
    }

    void CylinderObject::onResize(int width, int height) {
    }

    void CylinderObject::onKey(int key, int scancode, int action, int mods) {
        switch (key) {
            case GLFW_KEY_K:
                if (action == GLFW_PRESS) {
                    bezierStart();
                    bezierUpdate();
                    bezierUpdate();
                }
                break;
        }
    }

    void CylinderObject::onMouseButton(int button, int action, int mods) {
    }

    void CylinderObject::onExit() {

    }

    CylinderObject::CylinderObject(char *name) : IObject(name) {
        mesh_ = std::make_shared<CylinderMesh>(0.3f, 1.0f, 10000, glm::vec3(0.0f, 1.0f, 0.0f));
        transform_.setPosition(glm::vec3(-1, 0, 0));
        transform_.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        transform_.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        moving_ = false;
        t_ = 0;
    }

    void CylinderObject::bezierMove(float t) {
        getTransform()->setPosition(bezier_.cubicPositionBezierCurve(t));
        getTransform()->setRotation(bezier_.cubicRotationBezierCurve(t));
    }

    void CylinderObject::bezierStart() {
        auto transformPos = getTransform()->getPosition();
        auto transformRot = getTransform()->getRotation();
        bezier_.add(transformPos, transformRot);
        bezier_.add(glm::vec3(transformPos.x, transformPos.y, transformPos.z + 10),
                    glm::quat(0.0707372, 0.0, 0.0, 0.997495));
        bezier_.add(glm::vec3(transformPos.x + 10, transformPos.y, transformPos.z + 10),
                    glm::quat(0.0707372, 0.0, 0.0, 0.997495));
        bezier_.add(glm::vec3(transformPos.x + 10, transformPos.y, transformPos.z), transformRot);
        moving_ = true;
        t_ = 0;
    }

    void CylinderObject::bezierUpdate() {
        auto transformPos = bezier_.getLastPos();
        auto transformRot = bezier_.getLastRot();
        bezier_.add(glm::vec3(transformPos.x, transformPos.y, transformPos.z + 10),
                    glm::quat(0.0707372, 0.0, 0.0, 0.997495));
        bezier_.add(glm::vec3(transformPos.x + 10, transformPos.y, transformPos.z + 10),
                    glm::quat(0.0707372, 0.0, 0.0, 0.997495));
        bezier_.add(glm::vec3(transformPos.x + 10, transformPos.y, transformPos.z), transformRot);
        moving_ = true;
    }
} // entre_portais