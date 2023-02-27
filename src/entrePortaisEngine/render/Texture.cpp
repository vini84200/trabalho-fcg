#include "entrePortaisEngine/render/Texture.hpp"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "spdlog/spdlog.h"

namespace entre_portais {
    Texture::Texture(std::string name)
            : name_(name) {
        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::string path = "assets/textures/" + name;
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            spdlog::error("Failed to load texture {} at {}", name, path);
        }

        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);

    }

    void Texture::Bind() const {
        glBindTexture(GL_TEXTURE_2D, id_);
    }

    void Texture::Unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    unsigned int Texture::GetTextureID() const {
        return id_;
    }

    ImTextureID Texture::GetImTextureID() const {
        return (ImTextureID) id_;
    }
} // entre_portais