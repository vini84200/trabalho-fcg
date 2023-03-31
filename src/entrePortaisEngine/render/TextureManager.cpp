#include "entrePortaisEngine/render/TextureManager.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "utils/utils.hpp"

namespace entre_portais {

    TextureManager *TextureManager::instance_ = nullptr;

    TextureManager &TextureManager::instance() {
        if (instance_ == nullptr) {
            instance_ = new TextureManager();
        }
        return *instance_;
    }


    Texture TextureManager::getTextureSync(std::string name) {
        // Is loading
        const bool isLoading = texturesLoading_.find(name) != texturesLoading_.end();
        if (isLoading) {
            return getTextureAsync(name);
        }
        // Is not loading, is it already loaded?
        const bool isLoaded = textures_.find(name) != textures_.end();
        if (isLoaded) {
            return textures_.at(name);
        }
        // It is not loaded, so create it Sync
        return createTextureSync(name);

    }

    Texture TextureManager::getTextureAsync(std::string name) {
        // Is loading?
        const bool isLoading = texturesLoading_.find(name) != texturesLoading_.end();
        if (isLoading) {
            // Is it ready?
            dataToLoadMutex_->lock();
            const bool isReady = dataToLoad_.find(name) != dataToLoad_.end();
            dataToLoadMutex_->unlock();
            if (isReady) {
                return doLoad(name);
            }
        }
        // Is it already loaded?
        if (textures_.find(name) != textures_.end()) {
            return textures_.at(name);
        }

        // It is not loaded, so create it Async
        return createTextureTask(name);
    }

    Texture TextureManager::createTextureSync(std::string name) {
        // Assume that the texture is not loaded
        // Nor is it loading
        assert(textures_.find(name) == textures_.end());
        assert(texturesLoading_.find(name) == texturesLoading_.end());
        // Load the texture
        Texture texture = Texture(name);
        textures_.insert({name, texture});
        return texture;
    }

    Texture TextureManager::createTextureTask(std::string name) {
        // Assume that the texture is not loaded
        // Nor is it loading
        assert(textures_.find(name) == textures_.end());
        assert(texturesLoading_.find(name) == texturesLoading_.end());

        texturesLoading_.insert(name);
        TaskManager::getInstance()->addTask<TextureLoadingTask>(name);
        Texture texture = Texture();
        texture.setName(name);
        textures_.insert({name, texture});
        return texture;
    }

    Texture TextureManager::doLoad(std::string &name) {
        TextureData data = dataToLoad_.at(name);
        // Check if texture is already created
        if (textures_.find(name) != textures_.end()) {
            Texture &tex = textures_.at(name);
            tex.setData(data);
            dataToLoadMutex_->lock();
            dataToLoad_.erase(name);
            dataToLoadMutex_->unlock();
            return textures_.at(name);
        } else {
            Texture texture(name, data);
            textures_.emplace(name, texture);
            dataToLoadMutex_->lock();
            dataToLoad_.erase(name);
            dataToLoadMutex_->unlock();
            return texture;
        }
    }

    void TextureManager::LoadTexture(std::string name) {
        // Check if texture is already created
        if (textures_.find(name) != textures_.end()) {
            return;
        }
        // Check if texture is already loading
        if (texturesLoading_.find(name) != texturesLoading_.end()) {
            return;
        }
        getTextureAsync(name);

    }

    Texture TextureManager::getCubeMapTexture(char *name) {
        if (cubeMapTextures_.find(name) == cubeMapTextures_.end()) {
            return createCubeMapTexture(name);
        }
    }

    Texture TextureManager::createCubeMapTexture(std::string name) {
        return Texture();
    }

    void TextureManager::addLoadedData(std::string name, TextureData data) {
        dataToLoadMutex_->lock();
        dataToLoad_.insert({name, data});
        dataToLoadMutex_->unlock();
    }

    TextureManager::TextureManager() {
        dataToLoadMutex_ = new std::mutex();
    }

    TaskRunResult TextureLoadingTask::Run() {
        std::string path = "assets/textures/" + nameToLoad_;
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
        if (data) {
            TextureData textureData{};
            textureData.data = data;
            textureData.width = width;
            textureData.height = height;
            textureData.nrChannels = nrChannels;

            TextureManager &tm = TextureManager::instance();
            tm.addLoadedData(nameToLoad_, textureData);
            spdlog::info("Loaded texture {} at {}", nameToLoad_, path);
        } else {
            spdlog::error("Failed to load texture {} at {}", nameToLoad_, path);
        }
        return TaskRunResult::SUCCESS;
    }
} // entre_portais