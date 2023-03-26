#ifndef ENTREPORTAIS_TEXTUREMANAGER_HPP
#define ENTREPORTAIS_TEXTUREMANAGER_HPP

#include <string>
#include <unordered_map>
#include "Texture.hpp"

namespace entre_portais {

    class TextureManager {
    public:
        static TextureManager &instance();

        Texture getTexture(std::string name);

        void LoadTexture(std::string name);

        Texture getCubeMapTexture(char *name);

    private:
        TextureManager() = default;

        Texture createTexture(std::string name);


        static TextureManager *instance_;
        std::unordered_map<std::string, Texture> textures_;
        std::unordered_map<std::string, Texture> cubeMapTextures_;

        Texture createCubeMapTexture(std::string name);
    };

} // entre_portais

#endif //ENTREPORTAIS_TEXTUREMANAGER_HPP
