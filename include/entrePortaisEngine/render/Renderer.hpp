#ifndef ENTREPORTAIS_RENDERER_H
#define ENTREPORTAIS_RENDERER_H

#include "Camera.hpp"
#include "IRenderable.hpp"
#include "Shader.hpp"
#include "ShadersManager.hpp"
#include "unordered_map"
#include "FrameBuffer.hpp"
#include "VertexArrayBuffer.hpp"

namespace entre_portais {
    class IRenderable;

    class PointLight;

    enum RenderPass : int32_t {
        BACKGROUND = 0b00000001,
        PREPASS = 0b00000010,
        FOREGROUND = 0b00000100,
        LIGHTING = 0b00001000,
        TRANSPARENCY = 0b00010000,
        POSTPROCESS = 0b00100000,
        UI = 0b01000000,
    };

    enum HDRMode : int32_t {
        NONE = 0,
        REINHARD = 1,
        EXTENDED_REINHARD = 2,
        EXPOSURE = 3,
        FILMIC = 4,
        UNCHARTED2_SP = 5,
        ACES = 6,
        ACES2 = 7,
        REINHARD_LUMINANCE = 8,
    };


    using RenderPasses = int32_t;

    bool isInPass(RenderPass pass, RenderPasses passes);

    int getPassesCount(RenderPasses passes);

    using RenderableMap = std::unordered_map<int, IRenderable *>;
    using ShadersToRenderablesMap = std::unordered_map<int, RenderableMap>;
    using PassesToShadersMap = std::unordered_map<RenderPass, ShadersToRenderablesMap>;

    struct RenderableData {
        IRenderable *renderable;
        int shaderID;
        RenderPasses passes_to_render;
    };


    class Renderer : public std::enable_shared_from_this<Renderer> {
    public:
        Renderer();

        ~Renderer();

        void render(Camera *camera);

        void setLightsOnShader(Shader *shader, Camera *camera);

        void onWindowResize(int width, int height);

        int submit(IRenderable *renderable, int shaderID, RenderPasses passes_to_render);

        int submit(IRenderable *renderable, Shader shader, RenderPasses passes_to_render);

        void unSubmit(int id);

        int addPointLight(PointLight *light);

        void removePointLight(int id);

        void renderImGui();

    private:
        int lastID_ = 0;
//        std::unordered_map<int, std::unordered_map<int, IRenderable *>> renderables_;
        PassesToShadersMap renderables_;
        std::unordered_map<int, RenderableData> renderableData_;

        std::shared_ptr<FrameBuffer> frameBuffer;
        std::shared_ptr<Shader> postProcessShader;

        std::shared_ptr<VertexArrayBuffer> quadVAO_;

        // Configure post-processing
        HDRMode hdr = HDRMode::EXTENDED_REINHARD;
        bool bloom = true;
        bool gammaCorrection = true;
        float exposure = 1.0f;
        glm::vec4 whiteColor;

        // Directional light
        glm::vec3 lightDir = glm::normalize(glm::vec3(.2f, -1.f, 0.0f));
        glm::vec3 lightIntensity = glm::vec3(1.0f, 1.0f, 1.0f);
        float lightAmbient = 0.06f;

        std::unordered_map<int, PointLight *> pointLights_;

        void createQuadVAO();

    };

}  // namespace entre_portais

#endif  // ENTREPORTAIS_RENDERER_H
