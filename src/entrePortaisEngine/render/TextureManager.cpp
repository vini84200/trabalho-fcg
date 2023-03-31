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
        if (textures_.find(name) == textures_.end()) {
            return createTextureSync(name);
        }
        if (textures_.at(name).valid_ == false) {
            return createTextureTask(name);
        }
        return textures_.at(name);
    }

    Texture TextureManager::getTextureOrWait(std::string name) {
        if (textures_.find(name) == textures_.end() || textures_.at(name).valid_ == false) {
            return createTextureTask(name);
        }
        return textures_.at(name);
    }

    Texture TextureManager::createTextureSync(std::string name) {
        const bool isLoaded = dataToLoad_.find(name) != dataToLoad_.end();
        if (isLoaded) {
            TextureData const data = dataToLoad_.at(name);
            Texture texture(name, data);
            textures_.emplace(name, texture);
            return texture;
        }
        const bool isAlreadyLoading = texturesLoading_.find(name) != texturesLoading_.end();
        if (isAlreadyLoading) {
            return getTextureOrWait(name);
        }
        Texture texture = Texture(name);
        textures_.insert({name, texture});
        return texture;
    }

    Texture TextureManager::createTextureTask(std::string name) {

        // Check if data is ready
        dataToLoadMutex_->lock();
        const bool isReady = dataToLoad_.find(name) != dataToLoad_.end();
        dataToLoadMutex_->unlock();
        if (isReady) {
            // It is ready, so create the texture
            TextureData data = dataToLoad_.at(name);
            // Check if texture is already created
            if (textures_.find(name) != textures_.end()) {
                textures_.at(name).setData(data);
                return textures_.at(name);
            } else {
                Texture texture(name, data);
                textures_.emplace(name, texture);
                return texture;
            }
        }

        // Check if the task is already created
        const bool isAlreadyLoading = texturesLoading_.find(name) != texturesLoading_.end();
        if (isAlreadyLoading) {
            // Check if texture is already created
            if (textures_.find(name) != textures_.end()) {
                return textures_.at(name);
            } else {
                Texture texture = {};
                textures_.insert({name, texture});
                return texture;
            }
        }

        // It is not loaded, so create task to Load
        texturesLoading_.insert(name);
        TaskManager::getInstance()->addTask<TextureLoadingTask>(name);
        // Check if texture is already created
        if (textures_.find(name) != textures_.end()) {
            return textures_.at(name);
        } else {
            Texture texture = {}; // This is an empty texture, it will be filled when the task is finished
            textures_.insert({name, texture});
            return texture;
        }
    }

    void TextureManager::LoadTexture(std::string name) {
        const bool isAlreadyLoaded = textures_.contains(name);
        if (isAlreadyLoaded) {
            return;
        }

        const bool isAlreadyLoading = texturesLoading_.contains(name);
        if (isAlreadyLoading) {
            // Assert this is running on the main thread
            assert(utils::is_opengl_thread() && "This function should only be called from the main thread");
            // Check if data is ready
            dataToLoadMutex_->lock();
            const bool isReady = dataToLoad_.count(name) > 0;
            dataToLoadMutex_->unlock();
            if (isReady) {
                // It is ready, so create the texture
                TextureData data = dataToLoad_.at(name);
                Texture texture(name, data);
                textures_.emplace(name, texture);
            }


            return;
        }
        // Create task to Load
        texturesLoading_.emplace(name);
        TaskManager::getInstance()->addTask<TextureLoadingTask>(name);

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