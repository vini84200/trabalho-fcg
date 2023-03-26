#include "entrePortaisEngine/render/FrameBuffer.hpp"
#include "glad/glad.h"
#include "spdlog/spdlog.h"

namespace entre_portais {
    FrameBuffer::FrameBuffer() : FrameBuffer(800, 600) {}

    void FrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void FrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    unsigned int FrameBuffer::getColorAttachment() {
        return colorBuffer;
    }

    int FrameBuffer::getWidth() const {
        return width;
    }

    int FrameBuffer::getHeight() const {
        return height;
    }

    FrameBuffer::FrameBuffer(int width, int height) : width(width), height(height) {
        spdlog::debug("FrameBuffer constructor");

        // FONTE: https://learnopengl.com/Advanced-OpenGL/Framebuffers
        glGenFramebuffers(1, &fbo);

        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            spdlog::warn("Framebuffer is not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    FrameBuffer::~FrameBuffer() {
        spdlog::debug("FrameBuffer destructor");
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &colorBuffer);
        glDeleteRenderbuffers(1, &rbo);
    }


} // entre_portais