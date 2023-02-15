#ifndef ENTREPORTAIS_RENDERER_H
#define ENTREPORTAIS_RENDERER_H

#include "../Camera.hpp"
#include "IRenderable.hpp"
#include "../Shader.hpp"
#include "../ShadersManager.hpp"

namespace entre_portais
{
  class IRenderable;

  class Renderer : public std::enable_shared_from_this<Renderer>
  {
   public:
    Renderer();

    void render(Camera* camera);

    int submit(IRenderable *renderable, int shaderID);

    int submit(IRenderable *renderable, Shader shader);

    void unSubmit(int id, int shaderID);

   private:
    int lastID_ = 0;
    std::map<int, std::map<int, IRenderable*>> renderables_;
  };

}  // namespace entre_portais

#endif  // ENTREPORTAIS_RENDERER_H
