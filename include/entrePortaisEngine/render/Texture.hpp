#ifndef ENTREPORTAIS_TEXTURE_HPP
#define ENTREPORTAIS_TEXTURE_HPP

#include <string>
#include "imgui.h"

namespace entre_portais {

    class Texture {
    public:
        enum class Type {
            DIFFUSE,
            SPECULAR,
            NORMAL,
            AMBIENT,
            EMISSIVE,
            SHININESS,
            OPACITY,
            DISPLACEMENT,
            LIGHTMAP,
            REFLECTION,
        }; //TODO: Use
        unsigned int GetTextureID() const;

        ImTextureID GetImTextureID() const;

        std::string getName() const;

        void Bind() const;

        void Unbind() const;

        Texture() = default;

    private:
        Texture(std::string name);

        bool valid_ = false;
        unsigned int id_;
        std::string name_;

        friend class TextureManager;

    };

} // entre_portais

#endif //ENTREPORTAIS_TEXTURE_HPP
