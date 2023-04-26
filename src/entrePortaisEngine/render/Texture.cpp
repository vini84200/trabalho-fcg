#include "entrePortaisEngine/render/TextureHandle.hpp"
#include "entrePortaisEngine/render/TextureManager.hpp"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "spdlog/spdlog.h"

namespace entre_portais {
    void ITexture::Bind() {
        Bind(0);
    }

    void ITexture::Bind(unsigned int slot) {
        if (!isInitialized()) {
            initialize();
        }
        if (!isLoaded()) {
            load();
        }
        if (slot > MAX_TEXTURE_SLOTS) {
            spdlog::error("Trying to bind texture {} to slot {}, but only {} slots are available", GetName(), slot,
                          MAX_TEXTURE_SLOTS + 1);
            return;
        }
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(GetID()));
    }

    void ITexture::Unbind() {
        if (GetID() == 0) {
            spdlog::error("Trying to unbind texture {}, but it is not bound", GetName());
            return;
        }

    }
} // entre_portais