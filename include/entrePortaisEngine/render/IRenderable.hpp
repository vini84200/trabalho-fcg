#ifndef ENTREPORTAIS_IRENDERABLE_H
#define ENTREPORTAIS_IRENDERABLE_H

#include <memory>
#include "Shader.hpp"
#include "VertexArrayBuffer.hpp"

namespace entre_portais {
    class Renderer;

    enum RenderPass : int32_t;

    using RenderPasses = int32_t;

    struct RendererData {
        RendererData() = default;
    };

    class IRenderable {
    public:
        ~IRenderable();

        void submit(std::shared_ptr<Renderer> renderer, RenderPasses passes);

        void unSubmit();

        virtual void render(RenderPass current_pass) = 0;

        virtual void renderUi(VertexArrayBuffer *quadVao, Shader &shader) {};

        void loadShader(std::string name);

        void loadShader(Shader shader);

        Shader getShader();

        virtual glm::mat4 &getModelMatrix() = 0;

    private:
        bool isSubmitted_ = false;
        int id_;
        int shaderID_;
        bool hasShader_;
        std::shared_ptr<Renderer> renderer_;
    };

}  // namespace entre_portais

#endif  // ENTREPORTAIS_IRENDERABLE_H
