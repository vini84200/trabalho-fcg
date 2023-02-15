//
// Created by barbu on 13/02/2023.
//

#include "entrePortaisEngine/render/IRenderable.hpp"

namespace entre_portais
{
  void IRenderable::submit(std::shared_ptr<Renderer> renderer)
  {
    if (!hasShader_){
      throw std::runtime_error("Voce esqueceu de definir o shader do renderable");
    }
    renderer_ = renderer;
    id_ = renderer->submit(this, shaderID_);
    isSubmitted_ = true;
  }

  IRenderable::~IRenderable()
  {
    if (isSubmitted_){
      unSubmit();
    }
  }

  void IRenderable::loadShader(std::string name)
  {
    auto sm = ShadersManager::getInstance();
    loadShader(sm->getShader(name));
  }

  void IRenderable::loadShader(Shader shader)
  {
    hasShader_ = true;
    shaderID_ = shader.getID();
  }

  Shader IRenderable::getShader()
  {
    auto sm = ShadersManager::getInstance();
    return sm->getShaderByID(shaderID_);
  }

  void IRenderable::unSubmit()
  {
    renderer_->unSubmit(0, 0);
    isSubmitted_ = false;
  }
}  // namespace entre_portais