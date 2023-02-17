#include "entrePortaisEngine/Camera.hpp"

#include "entrePortaisEngine/IScene.hpp"
#include "utils/matrices.h"

namespace entre_portais
{


  void Camera::renderImGui()
  {
    if (ImGui::TreeNode(getName()))
    {
      if (ImGui::TreeNode("Transform"))
      {
        ImGui::DragFloat3("Position", transform_.getPositionPtr(), 0.1f);
        ImGui::DragFloat3("Rotation", transform_.getRotationPtr(), 0.1f);
        ImGui::DragFloat3("Scale", transform_.getScalePtr(), 0.1f);
        ImGui::TreePop();
      }
      if (ImGui::TreeNode("Camera Options")){
        ImGui::DragFloat("Far", &far_);
        ImGui::DragFloat("Near", &near_);
        ImGui::DragFloat("Aspect Ratio", &aspectRatio_);
        ImGui::DragFloat("Field of View", &fov_);
        ImGui::TreePop();
      }
      if (ImGui::TreeNode("Camera Log")){
        glm::vec4 viewVector = getViewVector();
        ImGui::Text("View Vector: x: %f y: %f z: %f w: %f", viewVector.x, viewVector.y, viewVector.z, viewVector.w);
        glm::mat4 viewMatrix = getViewMatrix();
        ImGui::Text("View Matrix:\n");
        ImGui::Text("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0], viewMatrix[3][0]);
        ImGui::Text("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], viewMatrix[3][1]);
        ImGui::Text("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2], viewMatrix[3][2]);
        ImGui::Text("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", viewMatrix[0][3], viewMatrix[1][3], viewMatrix[2][3], viewMatrix[3][3]);
        glm::mat4 projectionMatrix = getProjectionMatrix();
        ImGui::Text("Projection Matrix:\n");
        ImGui::Text("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n",
            projectionMatrix[0][0],
            projectionMatrix[1][0],
            projectionMatrix[2][0],
            projectionMatrix[3][0]);
        ImGui::Text("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n",
            projectionMatrix[0][1],
            projectionMatrix[1][1],
            projectionMatrix[2][1],
            projectionMatrix[3][1]);
        ImGui::Text("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n",
            projectionMatrix[0][2],
            projectionMatrix[1][2],
            projectionMatrix[2][2],
            projectionMatrix[3][2]);
        ImGui::Text("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n",
            projectionMatrix[0][3],
            projectionMatrix[1][3],
            projectionMatrix[2][3],
            projectionMatrix[3][3]);
        ImGui::TreePop();
      }
      if (ImGui::TreeNode("Children"))
      {
        for (auto &child : children_)
        {
          child->renderImGui();
        }
        ImGui::TreePop();
      }
      ImGui::TreePop();
    }
  }

  Camera::Camera(char* name, float fov, float n, float f, float aspectRatio) : IGameNode(name)
  {
    fov_ = fov;
    near_ = n;
    far_ = f;
    aspectRatio_ = aspectRatio;
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
  void Camera::setUpCamera()
  {
    // TODO
  }

  void Camera::configureShaderUniforms(Shader shader)
  {
    shader.setUniformMat4("view", getViewMatrix());
    shader.setUniformMat4("projection", getProjectionMatrix());
  }

  bool entre_portais::Camera::hasScene()
  {
    return !scene_.expired();
  }
  glm::mat4 Camera::getProjectionMatrix()
  {
    return matrices::Matrix_Perspective(fov_, aspectRatio_, -near_, -far_);
  }
  void Camera::onResize(int width, int height)
  {
    aspectRatio_ = ((float) width) / ((float) height);
  }
  glm::mat4 Camera::getViewMatrix()
  {
    return matrices::Matrix_Camera_View(transform_.getPosition(), getViewVector(), glm::vec4 (0, 1, 0, 0));
  }

  glm::vec4 Camera::getViewVector()
  {
    return glm::vec4 (
        cos(transform_.ry),
        sin(transform_.ry)*cos(transform_.rz),
        sin(transform_.ry)*sin(transform_.rz),
        0
        );
  }

}