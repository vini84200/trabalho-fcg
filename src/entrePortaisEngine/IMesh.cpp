#include "entrePortaisEngine/IMesh.hpp"

namespace entre_portais{
  IMesh::IMesh(std::string shaderName)
  {
    SetShader(shaderName);
  }
}