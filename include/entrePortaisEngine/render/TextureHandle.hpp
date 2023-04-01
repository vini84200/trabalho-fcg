#ifndef ENTREPORTAIS_TEXTUREHANDLE_HPP
#define ENTREPORTAIS_TEXTUREHANDLE_HPP

#include <string>
#include "imgui.h"
#include "stb_image.h"
#include <memory>

#define MAX_TEXTURE_SLOTS 31
namespace entre_portais {

    struct TextureData;
    class TextureHandle;

    class ITexture {
    public:
        virtual ~ITexture() = default;
        virtual unsigned int GetID() const = 0;
        virtual ImTextureID GetImTextureID() const = 0;
        virtual std::string GetName() const = 0;

        virtual void initialize() = 0;
        virtual bool isInitialized() const = 0;
        virtual void load() = 0;
        virtual bool isLoaded() const = 0;

        virtual void Bind();
        virtual void Bind(unsigned int slot);
        virtual void Unbind();
    };

    class TextureHandle
    {
    public:
        inline auto GetImTextureID() const -> ImTextureID { return texture_->GetImTextureID(); }
        inline void Bind() const { texture_->Bind(); }
        inline void Bind(unsigned int slot) const { texture_->Bind(slot); }
        inline void Unbind() const { texture_->Unbind(); }

        inline int GetID() const { return texture_->GetID(); }
        inline std::string GetName() const { return texture_->GetName(); }

        inline bool operator==(const TextureHandle &other) const { return texture_ == other.texture_; }
        inline bool operator!=(const TextureHandle &other) const { return texture_ != other.texture_; }

        inline bool operator==(const ITexture &other) const { return texture_.get() == &other; }
        inline bool operator!=(const ITexture &other) const { return texture_.get() != &other; }
        bool isLoaded() const { return texture_->isLoaded(); }

        inline std::shared_ptr<ITexture> getRawTexture() const { return texture_;}
        inline TextureHandle(TextureHandle &&other) noexcept : texture_(std::move(other.texture_)) {
        }
        inline TextureHandle(const TextureHandle &other) = default;
        inline TextureHandle &operator=(const TextureHandle &other) = default;
    private:
        explicit TextureHandle(std::shared_ptr<ITexture> texture) : texture_(std::move(texture)) {}

        std::shared_ptr<ITexture> texture_;
        friend class TextureManager;
    };


} // entre_portais

#endif  // ENTREPORTAIS_TEXTUREHANDLE_HPP
