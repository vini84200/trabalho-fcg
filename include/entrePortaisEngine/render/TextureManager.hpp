#ifndef ENTREPORTAIS_TEXTUREMANAGER_HPP
#define ENTREPORTAIS_TEXTUREMANAGER_HPP

#include <string>
#include <unordered_map>
#include <set>
#include "entrePortaisEngine/tasks/ITask.hpp"
#include "glad/glad.h"
#include "entrePortaisEngine/render/Texture.hpp"
#include "spdlog/spdlog.h"

namespace entre_portais {

    struct TextureData {
        unsigned char *data;
        int width;
        int height;
        int nrChannels;
    };

    class TextureManager {
    public:
        static TextureManager &instance();


        Texture getTextureSync(std::string name);

        void LoadTexture(std::string name);

        Texture getCubeMapTexture(char *name);

        void addLoadedData(std::string name, TextureData data);

    private:
        TextureManager();

        // Delete copy constructor and assignment operator
        TextureManager(TextureManager const &) = delete;

        void operator=(TextureManager const &) = delete;

        Texture createTextureSync(std::string name);


        static TextureManager *instance_;
        std::unordered_map<std::string, Texture> textures_;
        std::unordered_map<std::string, Texture> cubeMapTextures_;

        std::unordered_map<std::string, TextureData> dataToLoad_;
        std::mutex *dataToLoadMutex_;

        std::set<std::string> texturesLoading_;

        Texture createCubeMapTexture(std::string name);

        Texture getTextureOrWait(std::string name);

        Texture createTextureTask(std::string name);
    };


    class TextureLoadingTask : public entre_portais::ITask {
    public:
        TextureLoadingTask(std::string nameToLoad) : nameToLoad_(nameToLoad) {}

        TaskRunResult Run() override;

        void OnFinish() override {

        }

        void OnCancel() override {

        }

        std::string nameToLoad_;
    };

} // entre_portais

#endif //ENTREPORTAIS_TEXTUREMANAGER_HPP
