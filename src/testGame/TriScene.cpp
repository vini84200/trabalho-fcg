
#include "testGame/TriScene.hpp"

#include "entrePortaisEngine/Camera.hpp"
#include "glad/glad.h"
#include "testGame/TriObject.hpp"


namespace entre_portais
{
  TriScene::TriScene() : IScene("TriScene")
  {
  }

  void TriScene::initialize()
  {
    logger_.getLogger()->info("Initializing TriScene");
    char *name1 = "TriObject 1";
    char *name2 = "TriObject 2";
    auto obj = std::make_shared<TriObject>(name1);
    auto obj2 = std::make_shared<TriObject>(name2);
    addChild(obj);
    obj2->getTransform()->setPosition(glm::vec3(0.3f, 0.3f, 0.0f));
    obj2->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    addChild(obj2);

    char *name3 = "Child of TriObject 2";
    auto obj3 = std::make_shared<TriObject>(name3);
    obj3->getTransform()->setPosition(glm::vec3(0.3f, 0.3f, 0.0f));
    obj3->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    obj2->addChild(obj3);

    char *name4 = "Child of Child of TriObject 2";
    auto obj4 = std::make_shared<TriObject>(name4);
    obj4->getTransform()->setPosition(glm::vec3(0.3f, 0.3f, 0.0f));
    obj4->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    obj3->addChild(obj4);

    char *camera_name = "Camera";
    auto camera = std::make_shared<Camera>(camera_name);
    addChild(camera);
    setCamera(camera);
  }

  TriScene::~TriScene() = default;

  void TriScene::update(double deltaTime)
  {
  }

  void TriScene::onExit()
  {
  }

  void TriScene::onKey(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
  {
  }

  void TriScene::onResize(int /*width*/, int /*height*/)
  {
  }

  void TriScene::onMouseButton(int /*button*/, int /*action*/, int /*mods*/)
  {
  }

}  // namespace entre_portais