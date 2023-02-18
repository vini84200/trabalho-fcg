#include "entrePortaisEngine/ShadersManager.hpp"
#include "entrePortaisEngine/Shader.hpp"

namespace entre_portais
{
  ShadersManager* ShadersManager::instance_ = nullptr;
  ShadersManager* ShadersManager::getInstance(){
    if(instance_ == nullptr){
      instance_ = new ShadersManager();
    }
    return instance_;
  }
  Shader ShadersManager::createShader(std::string name)
  {
    int id = ++lastID_;
    std::string arqVer = "assets/shaders/" + name + ".vert";
    std::string arqFrag = "assets/shaders/" + name + ".frag";
    Shader shader(arqVer.c_str(), arqFrag.c_str());
    shader.setID(id);
    names_.emplace(name, id);
    shaders_.emplace(id, shader);
    return shader;

  }
  Shader ShadersManager::getShader(std::string name)
  {
    if (names_.find(name) == names_.end()){
      return createShader(name);
    }
    else{
      return shaders_.at(names_.at(name));
    }
  }
  Shader ShadersManager::getShaderByID(int id)
  {
    return shaders_.at(id);
  }
  ShadersManager::~ShadersManager()
  {
    for (auto [_,s]:shaders_){
      s.deleteShader();
    }
  }

}  // namespace entre_portais