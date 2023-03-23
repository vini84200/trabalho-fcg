//
// Created by barbu on 13/02/2023.
//

#include "entrePortaisEngine/render/Renderer.hpp"

#include "spdlog/spdlog.h"
#include "utils/matrices.h"

namespace entre_portais {

    Renderer::Renderer() {
        spdlog::info("renderer()");
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

        // POSTPROCESS
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 5, -1, "POSTPROCESS");

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