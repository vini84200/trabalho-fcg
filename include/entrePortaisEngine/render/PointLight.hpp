#ifndef ENTREPORTAIS_POINTLIGHT_HPP
#define ENTREPORTAIS_POINTLIGHT_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {
    class PointLight : public IObject {
    public:
        PointLight();

        PointLight(glm::vec3 position, glm::vec3 color, float intensity, float constant, float linear, float quadratic);

        ~PointLight() override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        void render(RenderPass current_pass) override;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        void registerLight();

        void unregisterLight();

        void CustomImGui() override;

        void shaderSetup(Shader *shader, int lightIndex) const;

        glm::vec3 color_;
        float intensity_;
        float ambientIntensity_;
        float constant_;
        float linear_;
        float quadratic_;
        float radius_;
        int lightId_ = -1;
    };

} // entre_portais

#endif //ENTREPORTAIS_POINTLIGHT_HPP
