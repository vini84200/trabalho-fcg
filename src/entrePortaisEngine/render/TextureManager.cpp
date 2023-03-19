#include "entrePortaisEngine/render/TextureManager.hpp"

namespace entre_portais {
    TextureManager *TextureManager::instance_ = nullptr;

    TextureManager &TextureManager::instance() {
        if (instance_ == nullptr) {
            instance_ = new TextureManager();
        }
        return *instance_;
    }

    Texture TextureManager::getTexture(std::string name) {
        if (textures_.find(name) == textures_.end()) {
            return createTexture(name);
        }
        return textures_.at(name);
    }

    Texture TextureManager::createTexture(std::string name) {
        Texture texture = Texture(name);
        textures_.insert({name, texture});
        return texture;
    }

    void TextureManager::LoadTexture(std::string name) {
        getTexture(name);

    }

    Texture TextureManager::getCubeMapTexture(char *name) {
        if (cubeMapTextures_.find(name) == cubeMapTextures_.end()) {
            return createCubeMapTexture(name);
        }
    }

    Texture TextureManager::createCubeMapTexture(std::string name) {
        return Texture();
    }
} // entre_portais