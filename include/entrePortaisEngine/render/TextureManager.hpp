#ifndef ENTREPORTAIS_TEXTUREMANAGER_HPP
#define ENTREPORTAIS_TEXTUREMANAGER_HPP

#include <set>
#include <string>
#include <unordered_map>

#include "entrePortaisEngine/render/TextureHandle.hpp"
#include "entrePortaisEngine/tasks/ITask.hpp"
#include "glad/glad.h"
#include "spdlog/spdlog.h"

namespace entre_portais {

    class TextureManager {
    public:
        static TextureManager &instance();

        // Delete copy constructor and assignment operator
        TextureManager(TextureManager const &) = delete;
        void operator=(TextureManager const &) = delete;

        TextureHandle getTexture(std::string name);

        void LoadTexture(std::string name);

    private:
        TextureManager();

        static TextureManager *instance_;
        std::unordered_map<std::string, TextureHandle> textures_;
        std::unordered_map<std::string, TextureHandle> cubeMapTextures_;

        TextureHandle createTexture(std::string name);
    };



} // entre_portais

#endif //ENTREPORTAIS_TEXTUREMANAGER_HPP
