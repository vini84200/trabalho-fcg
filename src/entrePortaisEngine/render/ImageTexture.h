#ifndef ENTREPORTAIS_IMAGETEXTURE_H
#define ENTREPORTAIS_IMAGETEXTURE_H


#include "entrePortaisEngine/render/TextureHandle.hpp"
#include "entrePortaisEngine/tasks/TaskHandler.hpp"

#define IMAGE_MISSING_SIZE 64
namespace entre_portais {
    class ImageTexture : public ITexture {
    public:
        ImageTexture(std::string path, std::string name);
        unsigned int GetID() const override;

        ImTextureID GetImTextureID() const override;

        std::string GetName() const override;

        void initialize() override;

        inline bool isInitialized() const override { return is_initialized_; }

        void load() override;

        inline bool isLoaded() const override;

    private:
        unsigned int texture_id_;
        std::string path_;
        std::string name_;

        TaskHandler task_handler_;
        bool is_initialized_ = false;
        bool is_loaded_ = false;

        void createOpenGLTexture();
    };
}


#endif //ENTREPORTAIS_IMAGETEXTURE_H
