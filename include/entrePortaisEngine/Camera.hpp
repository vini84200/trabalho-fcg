#ifndef ENTREPORTAIS_CAMERA_HPP
#define ENTREPORTAIS_CAMERA_HPP

#include <entrePortaisEngine/IGameNode.hpp>
#include <memory>
#include "entrePortaisEngine/ShadersManager.hpp"

namespace entre_portais
{

  class Camera : public IGameNode {
   public:
    Camera(char *name);
    ~Camera() override {};
    Camera(const Camera &other) = delete;
    Camera &operator=(const Camera &other) = delete;
    Camera(Camera &&other) = delete;
    Camera &operator=(Camera &&other) = delete;
    void renderImGui() override;
    void setUpCamera();
    void update(double deltaTime) override {};
    void initialize() override {};
    void onResize(int width, int height) override {};
    void onKey(int key, int scancode, int action, int mods) override {};
    void onMouseButton(int button, int action, int mods) override {};
    void onExit() override {};
    void CustomImGui() override {};
    std::shared_ptr<IScene> getScene() override;

    void setScene(std::shared_ptr<IScene> scene) override;

    bool hasScene() override;

   private:
    std::weak_ptr<IScene> scene_;
  };

}  // namespace entre_Portais

#endif  // ENTREPORTAIS_CAMERA_HPP
