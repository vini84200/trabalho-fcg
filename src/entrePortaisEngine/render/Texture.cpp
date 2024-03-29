#include "entrePortaisEngine/render/Texture.hpp"
#include "glad/glad.h"
#include "entrePortaisEngine/render/TextureManager.hpp"

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
        valid_ = true;

    }

    void Texture::Bind() const {
//        if (!valid_) {
//            spdlog::error("Trying to bind invalid texture {}", name_);
//            return;
//        }
        glBindTexture(GL_TEXTURE_2D, id_);
    }

    void Texture::Unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    unsigned int Texture::GetTextureID() const {
        if (!valid_) {
            spdlog::error("Trying to get id of invalid texture {}", name_);
            return 0;
        }
        return id_;
    }

    ImTextureID Texture::GetImTextureID() const {
        if (!valid_) {

            return 0;
        }
        return (ImTextureID) id_;
    }

    void Texture::Bind(unsigned int slot) const {
//        if (!valid_) {
//            spdlog::error("Trying to bind invalid texture {}", name_);
//            return;
//        }
        if (slot > 31) {
            spdlog::error("Trying to bind texture {} to slot {}, but only 32 slots are available", name_, slot);
            return;
        }
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id_);
    }

    Texture::Texture(std::string name, TextureData data) {
        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data.data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::string path = "assets/textures/" + name;
            spdlog::error("Failed to load texture {} at {}", name, path);
        }

        stbi_image_free(data.data);

    }

    Texture::Texture() {
        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void Texture::setData(TextureData data) {
        spdlog::info("Setting data for texture {}", name_);
        if (data.data) {
            glBindTexture(GL_TEXTURE_2D, id_);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
            glGenerateMipmap(GL_TEXTURE_2D); // TODO: generate mipmaps outside of the main thread
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data.data);
        } else {
            spdlog::error("Failed to load texture {} at {}", name_, "assets/textures/" + name_);
        }


    }

    void Texture::setName(std::string name) {
        name_ = name;
    }
} // entre_portais