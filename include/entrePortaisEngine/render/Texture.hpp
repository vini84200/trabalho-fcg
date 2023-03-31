#ifndef ENTREPORTAIS_TEXTURE_HPP
#define ENTREPORTAIS_TEXTURE_HPP

#include <string>
#include "imgui.h"
#include "stb_image.h"

namespace entre_portais {

    struct TextureData;

    class Texture {
    public:
        Texture(std::string name, TextureData data);

        unsigned int GetTextureID() const;

        ImTextureID GetImTextureID() const;

        std::string getName() const;

        void Bind() const;

        void Bind(unsigned int slot) const;

        void Unbind() const;

        Texture();

    private:
        Texture(std::string name);

        bool valid_ = false;
        unsigned int id_;
        std::string name_;

        friend class TextureManager;

        void setData(TextureData data);
    };

} // entre_portais

#endif //ENTREPORTAIS_TEXTURE_HPP
