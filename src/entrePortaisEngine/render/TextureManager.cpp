#include "entrePortaisEngine/render/TextureManager.hpp"
#include <memory>
#include "ImageTexture.h"
#include "entrePortaisEngine/render/TextureHandle.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "spdlog/spdlog.h"
#include "utils/utils.hpp"

namespace entre_portais {

    TextureManager *TextureManager::instance_ = nullptr;

    TextureManager &TextureManager::instance() {
        if (instance_ == nullptr) {
            instance_ = new TextureManager();
        }
        return *instance_;
    }


    TextureHandle TextureManager::getTexture(std::string name) {
        // If the texture is already loaded, return it
        if (textures_.find(name) != textures_.end()) {
            return textures_.at(name);
        }

        // If the texture is not loaded, load it
        return createTexture(name);

    }


    TextureManager::TextureManager() {
    }

    TextureHandle TextureManager::createTexture(std::string name) {
        auto handle = TextureHandle(
                std::make_shared<ImageTexture>(
                        utils::getTexturePath(name),
                        name
                )
        );
        textures_.insert({name, handle});
        return handle;
    }

    void TextureManager::LoadTexture(std::string name) {
        // If the texture is already loaded, return it
        if (textures_.find(name) != textures_.end()) {
            return;
        }

        // If the texture is not loaded, load it
        createTexture(name);

    }
} // entre_portais