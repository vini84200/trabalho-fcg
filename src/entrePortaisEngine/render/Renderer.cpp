#include "entrePortaisEngine/render/Renderer.hpp"

#include "spdlog/spdlog.h"
#include "utils/matrices.h"
#include "entrePortaisEngine/Window.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "entrePortaisEngine/render/PointLight.hpp"

#ifdef OLD_OPENGL
#define NOT_USING_OPENGL_DEBUG
#endif

#ifndef NOT_USING_OPENGL_DEBUG
#define OPENGL_DEBUG
#endif

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
        quadVAO_->addBufferToQueue(&vboPos);

        texPos.addAttribute(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        texPos.setData(quadVertices, sizeof(quadVertices));
        texPos.setUsage(GL_STATIC_DRAW);
        quadVAO_->addBufferToQueue(&texPos);

        quadVAO_->Commit();
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
            int width, height;
            glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
            frameBuffer = std::make_shared<FrameBuffer>(width, height);
        }
        if (postProcessShader.get() == nullptr) {
            postProcessShader = std::make_shared<Shader>(sm->getShader("postprocess"));
        }
        if (quadVAO_.get() == nullptr) {
            quadVAO_ = std::make_shared<VertexArrayBuffer>();
            createQuadVAO();
        }


        frameBuffer->bind();

        glViewport(0, 0, frameBuffer->getWidth(), frameBuffer->getHeight());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera->setUpCamera();
        // BACKGROUND
        glDepthMask(GL_FALSE);
        #ifdef OPENGL_DEBUG
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "BACKGROUND");
        #endif
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
        #ifdef OPENGL_DEBUG
        glPopDebugGroup();
        #endif


        // PREPASS
        #ifdef OPENGL_DEBUG
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "PREPASS");
        #endif


        #ifdef OPENGL_DEBUG
        glPopDebugGroup();
        #endif
        // FOREGROUND

        #ifdef OPENGL_DEBUG
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 2, -1, "FOREGROUND");
        #endif

        camera->setUpCamera();
        for (auto [shaderID, objs]: renderables_[FOREGROUND]) {
            auto shader = sm->getShaderByID(shaderID);
            shader.use();
            camera->configureShaderUniforms(shader);
            setLightsOnShader(&shader, camera);
            for (auto [_, obj]: objs) {
                shader.setUniformMat4("model", obj->getModelMatrix());
                obj->render(FOREGROUND);
            }
        }
        #ifdef OPENGL_DEBUG
        glPopDebugGroup();
        #endif

        // LIGHTING
        #ifdef OPENGL_DEBUG
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 3, -1, "LIGHTING");
        #endif

        #ifdef OPENGL_DEBUG
        glPopDebugGroup();
        #endif

        // TRANSPARENCY
        #ifdef OPENGL_DEBUG
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 4, -1, "TRANSPARENCY");
        #endif

        #ifdef OPENGL_DEBUG
        glPopDebugGroup();
        #endif

        frameBuffer->unbind();

        // POSTPROCESS
        #ifdef OPENGL_DEBUG
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 5, -1, "POSTPROCESS");
        #endif


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


        quadVAO_->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        quadVAO_->unbind();


#ifdef OPENGL_DEBUG
        glPopDebugGroup();
#endif

#ifdef OPENGL_DEBUG
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 6, -1, "UI");
#endif
        glClear(GL_DEPTH_BUFFER_BIT);
        VertexArrayBuffer *quadVAO = quadVAO_.get();
        for (auto [shaderID, objs]: renderables_[UI]) {
            Shader shader = sm->getShaderByID(shaderID);
            shader.use();
            camera->configureShaderUniformsForUi(shader);
            for (auto [_, obj]: objs) {
                obj->renderUi(quadVAO, shader);
            }
        }
#ifdef OPENGL_DEBUG
        glPopDebugGroup();
#endif
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

        if (ImGui::TreeNode("FrameBuffer")) {
            ImGui::Indent();
            ImGui::Text("Width: %d", frameBuffer->getWidth());
            ImGui::Text("Height: %d", frameBuffer->getHeight());
            ImGui::Unindent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lights")) {
            ImGui::Indent();
            if (ImGui::TreeNode("Directional Light")) {
                ImGui::Indent();
                ImGui::SliderFloat3("Direction", glm::value_ptr(lightDir), -1.0f, 1.0f);
                ImGui::DragFloat3("Intensity", glm::value_ptr(lightIntensity), 0.01f, 0.0f, 1.0f);
                ImGui::SliderFloat("Ambient", &lightAmbient, 0.0f, 1.0f);
                ImGui::Unindent();
                ImGui::TreePop();
            }
            ImGui::Text("Point Lights: %d / %d", pointLights_.size(), 10);
            ImGui::Unindent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Postprocess")) {
            ImGui::Indent();
            ImGui::Checkbox("HDR", &hdr);
            ImGui::Checkbox("Bloom", &bloom);
            ImGui::Checkbox("Gamma Correction", &gammaCorrection);
            ImGui::SliderFloat("Exposure", &exposure, -2.0f, 10.0f);
            ImGui::Unindent();

            ImGui::TreePop();
        }



    }

    void Renderer::setLightsOnShader(Shader *shader, Camera *camera) {
        shader->setUniformVec3("directionalLight.direction", lightDir);
        shader->setUniformVec3("directionalLight.base.intensity", lightIntensity);
        shader->setUniformFloat("directionalLight.base.ambient", lightAmbient);

        int maxLights = 10;
        int lightsCount = 0;
        lightsCount = std::min(maxLights, static_cast<int>(pointLights_.size()));
        shader->setUniformInt("pointLightsCount", lightsCount);
        int i = 0;
        for (const auto &[_, light]: pointLights_) {
            std::string lightName = "pointLights[" + std::to_string(lightsCount) + "]";
            light->shaderSetup(shader, i);
            i++;
            if (i >= maxLights) {
                break;
            }
        }

    }

    int Renderer::addPointLight(PointLight *light) {
        int id = ++lastID_;
        pointLights_.emplace(id, light);
        return id;
    }

    void Renderer::removePointLight(int id) {
        pointLights_.erase(id);
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