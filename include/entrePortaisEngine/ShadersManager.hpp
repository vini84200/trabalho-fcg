#ifndef ENTREPORTAIS_SHADERSMANAGER_H
#define ENTREPORTAIS_SHADERSMANAGER_H

#include <map>
#include <string>
#include "entrePortaisEngine/Shader.hpp"

namespace entre_portais
{

  class ShadersManager
  {
   public:
    static ShadersManager* getInstance();
    Shader getShader(std::string name);
    Shader getShaderByID(int id);

   private:
    ShadersManager() {};
    Shader createShader(std::string name);
    std::map<int, Shader> shaders_;
    std::map<std::string, int> names_;
    static ShadersManager* instance_;
    int lastID_ = 0;
  };

}  // namespace entre_portais

#endif  // ENTREPORTAIS_SHADERSMANAGER_H
