#include "entrePortaisEngine/render/PointLight.hpp"
#include "entrePortaisEngine/render/Renderer.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "utils/matrices.h"

namespace entre_portais {
    void PointLight::onKey(int key, int scancode, int action, int mods) {

    }

    void PointLight::onMouseButton(int button, int action, int mods) {

    }

    void PointLight::onExit() {

    }

    void PointLight::render(RenderPass current_pass) {
        IObject::render(current_pass);
    }

    void PointLight::update(float deltaTime) {
    }

    void PointLight::initialize() {
        registerLight();
    }

    void PointLight::onResize(int width, int height) {
    }

    void PointLight::registerLight() {
        auto renderer = IObject::getScene()->getRenderer();
        lightId_ = renderer->addPointLight(this);
        std::string name = "Point Light " + std::to_string(lightId_);
        setName(name.c_str());
    }

    PointLight::PointLight(glm::vec3 position, glm::vec3 color, float intensity, float constant, float linear,
                           float quadratic)
            : IObject("Point Light") {
        getTransform()->setPosition(position);
        color_ = color;
        intensity_ = intensity;
        constant_ = constant;
        linear_ = linear;
        quadratic_ = quadratic;
        ambientIntensity_ = 0.0f;
    }

    void PointLight::CustomImGui() {
        ImGui::Text("Point Light");
        ImGui::ColorEdit3("Color", glm::value_ptr(color_), ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float);
        ImGui::DragFloat("Intensity", &intensity_, 0.1f);
        ImGui::DragFloat("Constant", &constant_, 0.01f);
        ImGui::DragFloat("Linear", &linear_, 0.001f);
        ImGui::DragFloat("Quadratic", &quadratic_, 0.00001f);
    }

    void PointLight::shaderSetup(Shader *shader, int lightIndex) const {
        shader->setUniformFloatInArray("pointLights[].constant_attenuation", lightIndex, constant_);
        shader->setUniformFloatInArray("pointLights[].linear_attenuation", lightIndex, linear_);
        shader->setUniformFloatInArray("pointLights[].quadratic_attenuation", lightIndex, quadratic_);
        shader->setUniformFloatInArray("pointLights[].radius", lightIndex, radius_);
        shader->setUniformVec3InArray("pointLights[].position", lightIndex, matrices::PositionFromMatrix(modelMatrix_));
        shader->setUniformVec3InArray("pointLights[].base.intensity", lightIndex, color_ * intensity_);
        shader->setUniformFloatInArray("pointLights[].base.ambient", lightIndex, ambientIntensity_);
    }

    PointLight::PointLight() :
            PointLight(glm::vec3(0.0f), glm::vec3(1.0f), 1.0f, 1.0f, 0.09f, 0.032f) {

    }

    PointLight::~PointLight() {
        if (lightId_ != -1) {
            unregisterLight();
        }

    }

    void PointLight::unregisterLight() {
        if (lightId_ != -1) {
            if (!IObject::hasScene()) return;
            auto renderer = IObject::getScene()->getRenderer();
            renderer->removePointLight(lightId_);
            lightId_ = -1;
        }

    }
} // entre_portais