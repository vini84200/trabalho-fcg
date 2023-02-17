#include "entrePortaisEngine/IScene.hpp"

void entre_portais::IScene::renderImGui(bool *p_open) {
    if (ImGui::Begin(getName(), p_open)) {
        ImGui::Text("Scene");
        if (ImGui::CollapsingHeader("Objects")) {
            for (auto &child: children_) {
                child->renderImGui(nullptr);
            }
        }
        ImGui::End();
    } else {
        ImGui::End();
    }
}
void entre_portais::IScene::render()
{
  renderer_->render(camera_.get());
}
const std::shared_ptr<entre_portais::Renderer> &entre_portais::IScene::getRenderer() const
{
  return renderer_;
}
entre_portais::IScene::IScene(char *name) : IGameNode(name) {
  renderer_ = std::make_shared<Renderer>();
}