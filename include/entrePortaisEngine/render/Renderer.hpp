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

    enum RenderPass : int32_t {
        BACKGROUND = 0b00000001,
        PREPASS = 0b00000010,
        FOREGROUND = 0b00000100,
        LIGHTING = 0b00001000,
        TRANSPARENCY = 0b00010000,
        POSTPROCESS = 0b00100000,
        UI = 0b01000000,
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

        void onWindowResize(int width, int height);

        int submit(IRenderable *renderable, int shaderID, RenderPasses passes_to_render);

        int submit(IRenderable *renderable, Shader shader, RenderPasses passes_to_render);

        void unSubmit(int id);

        void renderImGui();

    private:
        int lastID_ = 0;
//        std::unordered_map<int, std::unordered_map<int, IRenderable *>> renderables_;
        PassesToShadersMap renderables_;
        std::unordered_map<int, RenderableData> renderableData_;

        std::shared_ptr<FrameBuffer> frameBuffer;
        std::shared_ptr<Shader> postProcessShader;

        std::shared_ptr<VertexArrayBuffer> quadVAO;

        // Configure post-processing
        bool hdr = true;
        bool bloom = true;
        bool gammaCorrection = true;
        float exposure = 1.0f;


        void createQuadVAO();

    };

}  // namespace entre_portais

#endif  // ENTREPORTAIS_RENDERER_H
