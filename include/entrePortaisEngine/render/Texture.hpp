#ifndef ENTREPORTAIS_TEXTURE_HPP
#define ENTREPORTAIS_TEXTURE_HPP

#include <string>

namespace entre_portais {

    class Texture {
    public:
        unsigned int getId() const;

        std::string getName() const;

        void Bind() const;

        void Unbind() const;

    private:
        Texture(std::string name);

        unsigned int id_;
        std::string name_;

        friend class TextureManager;
    };

} // entre_portais

#endif //ENTREPORTAIS_TEXTURE_HPP
