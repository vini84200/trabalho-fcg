#include "entrePortaisEngine/Camera.hpp"

#include "entrePortaisEngine/IScene.hpp"
#include "utils/matrices.h"

namespace entre_portais
{


  void Camera::renderImGui()
  {

  }

  Camera::Camera(char *name) : IGameNode(name)
  {

  }
  void Camera::setUpCamera()
  {
    // TODO
  }

  std::shared_ptr<entre_portais::IScene> entre_portais::Camera::getScene()
  {
    if (scene_.expired())
    {
      throw std::runtime_error("A cena não existe mais.");
    }
    return scene_.lock();
  }

  void entre_portais::Camera::setScene(std::shared_ptr<IScene> scene)
  {
    scene_ = scene;
  }

  bool entre_portais::Camera::hasScene()
  {
    return !scene_.expired();
  }

}