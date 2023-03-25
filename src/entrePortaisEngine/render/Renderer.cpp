#include "entrePortaisEngine/render/Renderer.hpp"

#include "spdlog/spdlog.h"
#include "utils/matrices.h"
#include "entrePortaisEngine/Window.hpp"

namespace entre_portais {

    Renderer::Renderer() {
        spdlog::info("renderer()");
    }

    void Renderer::createQuadVAO() {// Cria o quad para o postprocess renderizar em
// FONTE: https://learnopengl.com/Advanced-OpenGL/Framebuffers
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                -1.0f, 0.0f, 1.0f, 0.0f,};

        BufferBuilder vboPos, texPos;
        vboPos.addAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        vboPos.setData(quadVertices, sizeof(quadVertices));
        vboPos.setUsage(GL_STATIC_DRAW);
        quadVAO->addBufferToQueue(&vboPos);

        texPos.addAttribute(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        texPos.setData(quadVertices, sizeof(quadVertices));
        texPos.setUsage(GL_STATIC_DRAW);
        quadVAO->addBufferToQueue(&texPos);

        quadVAO->Commit();
    }

    int Renderer::submit(IRenderable *renderable, int shaderID, RenderPasses passes_to_render) {
        int id = ++lastID_;
//        renderables_[shaderID].emplace(id, renderable);
        for (int i = 0; i < 7; i++) {
            RenderPass pass = static_cast<RenderPass>(1 << i);
            if (isInPass(pass, passes_to_render)) {
                renderables_[pass][shaderID].emplace(id, renderable);
            }
        }
        renderableData_.emplace(id, RenderableData{renderable, shaderID, passes_to_render});
        return id;
    }

    void Renderer::render(Camera *camera) {
        auto sm = ShadersManager::getInstance();

        if (frameBuffer.get() == nullptr) {
            frameBuffer = std::make_shared<FrameBuffer>();
        }
        if (postProcessShader.get() == nullptr) {
            postProcessShader = std::make_shared<Shader>(sm->getShader("postprocess"));
        }
        if (quadVAO.get() == nullptr) {
            quadVAO = std::make_shared<VertexArrayBuffer>();
            createQuadVAO();
        }


        frameBuffer->bind();

        glViewport(0, 0, frameBuffer->getWidth(), frameBuffer->getHeight());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera->setUpCamera();
        // BACKGROUND
        glDepthMask(GL_FALSE);
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "BACKGROUND");
        for (auto [shaderID, objs]: renderables_[BACKGROUND]) {
            auto shader = sm->getShaderByID(shaderID);
            shader.use();
            camera->configureShaderUniforms(shader);
            // TODO: This is a temporary solution, we should have a way to configure the shader uniforms
            //  without having to know the shader's internals
            shader.setUniformMat4("view", glm::mat4(glm::mat3(camera->getViewMatrix())));
            for (auto [_, obj]: objs) {
                shader.setUniformMat4("model", obj->getModelMatrix());
                obj->render(BACKGROUND);
            }
        }
        glDepthMask(GL_TRUE);
        glPopDebugGroup();


        // PREPASS
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "PREPASS");

        glPopDebugGroup();
        // FOREGROUND
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 2, -1, "FOREGROUND");

        camera->setUpCamera();
        for (auto [shaderID, objs]: renderables_[FOREGROUND]) {
            auto shader = sm->getShaderByID(shaderID);
            shader.use();
            camera->configureShaderUniforms(shader);
            for (auto [_, obj]: objs) {
                shader.setUniformMat4("model", obj->getModelMatrix());
                obj->render(FOREGROUND);
            }
        }
        glPopDebugGroup();

        // LIGHTING
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 3, -1, "LIGHTING");

        glPopDebugGroup();

        // TRANSPARENCY
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 4, -1, "TRANSPARENCY");

        glPopDebugGroup();

        frameBuffer->unbind();

        // POSTPROCESS
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 5, -1, "POSTPROCESS");


        int width, height;
        glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        postProcessShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, frameBuffer->getColorAttachment());

        postProcessShader->setUniformBool("hdr", hdr);
        postProcessShader->setUniformBool("bloom", bloom);
        postProcessShader->setUniformBool("gammaCorrect", gammaCorrection);
        postProcessShader->setUniformFloat("exposure", exposure);


        quadVAO->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        quadVAO->unbind();


        glPopDebugGroup();

    }

    int Renderer::submit(IRenderable *renderable, Shader shader, RenderPasses passes_to_render) {
        return submit(renderable, shader.getID(), passes_to_render);
    }

    void Renderer::unSubmit(int id) {
        // Find the renderable and remove it
        auto it = renderableData_.find(id);
        if (it == renderableData_.end()) {
            spdlog::warn("Tentativa de remover um renderable que nao existe, id: {}", id);
            return;
        }
        RenderableData data = it->second;
        // First remove it from the renderables_ map
        for (int i = 0; i < 7; i++) {
            RenderPass pass = static_cast<RenderPass>(1 << i);
            if (isInPass(pass, data.passes_to_render)) {
                renderables_[pass][data.shaderID].erase(id);
            }
        }
        // Then remove it from the renderableData_ map
        renderableData_.erase(id);
    }

    Renderer::~Renderer() {
    }

    void Renderer::onWindowResize(int width, int height) {
        spdlog::info("Window resized to {}x{}", width, height);
        frameBuffer.reset(new FrameBuffer(width, height));
    }

    void Renderer::renderImGui() {
        ImGui::Begin("Renderer");
        ImGui::Text("Renderables: %d", lastID_);
        ImGui::Text("Passes:");
        ImGui::Indent();
        ImGui::Text("Background");
        ImGui::Text("Prepass");
        ImGui::Text("Foreground");
        ImGui::Text("Lighting");
        ImGui::Text("Transparency");
        ImGui::Text("Postprocess");
        ImGui::Unindent();

        if (ImGui::TreeNode("Postprocess")) {
            ImGui::Indent();
            ImGui::Checkbox("HDR", &hdr);
            ImGui::Checkbox("Bloom", &bloom);
            ImGui::Checkbox("Gamma Correction", &gammaCorrection);
            ImGui::SliderFloat("Exposure", &exposure, -2.0f, 10.0f);
            ImGui::Unindent();

            ImGui::TreePop();
        }


        ImGui::End();

    }


    bool isInPass(RenderPass pass, RenderPasses passes) {
        return (pass & passes) == pass;
    }

    int getPassesCount(RenderPasses passes) {
        int count = 0;
        for (int i = 0; i < 7; i++) {
            RenderPass pass = static_cast<RenderPass>(1 << i);
            if (isInPass(pass, passes)) {
                count++;
            }
        }
        return count;
    }
}  // namespace entre_portais