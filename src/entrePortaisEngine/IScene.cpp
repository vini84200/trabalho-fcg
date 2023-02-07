#include "entrePortaisEngine/IScene.hpp"

void entre_portais::IScene::renderImGui()
{
  if (ImGui::Begin(getName()))
  {
    ImGui::Text("Scene");
    if (ImGui::CollapsingHeader("Objects"))
    {
      for (auto &child : children_)
      {
        child->renderImGui();
      }
    }
    ImGui::End();
  }
  else
  {
    ImGui::End();
  }
}
