//
// Created by barbu on 13/02/2023.
//

#include "entrePortaisEngine/render/Renderer.hpp"

#include "spdlog/spdlog.h"
#include "utils/matrices.h"

namespace entre_portais
{

  Renderer::Renderer()
  {
    spdlog::info("renderer()");
  }

  int Renderer::submit(IRenderable *renderable, int shaderID)
  {
    int id = ++lastID_;
    renderables_[shaderID].emplace(id, renderable);
    return id;
  }

  void Renderer::render(Camera *camera)
  {
    auto sm = ShadersManager::getInstance();
    camera->setUpCamera();
    for (auto [shaderID, objs]: renderables_){
      auto shader = sm->getShaderByID(shaderID);
      shader.use();
      camera->configureShaderUniforms(shader);
      for (auto [_, obj]: objs){
        obj->render();
      }
    }
  }

  int Renderer::submit(IRenderable *renderable, Shader shader)
  {
    return submit(renderable, shader.getID());
  }

  void Renderer::unSubmit(int id, int shaderID)
  {
    auto mapID = renderables_.find(shaderID);
    mapID->second.erase(mapID->second.find(id));
    if (mapID->second.size() == 0){
      renderables_.erase(mapID);
    }
  }
}  // namespace entre_portais