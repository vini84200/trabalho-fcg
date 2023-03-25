//
// Created by vini84200 on 3/24/23.
//

#ifndef ENTREPORTAIS_FRAMEBUFFER_HPP
#define ENTREPORTAIS_FRAMEBUFFER_HPP

namespace entre_portais {

    class FrameBuffer {
    public:
        FrameBuffer();

        FrameBuffer(int width, int height);

        ~FrameBuffer();


        void bind();

        void unbind();

        unsigned int getColorAttachment();

    private:
        unsigned int fbo;
        unsigned int colorBuffer;
        unsigned int rbo;
        int width, height;
    public:
        int getWidth() const;

        int getHeight() const;

    };

} // entre_portais

#endif //ENTREPORTAIS_FRAMEBUFFER_HPP
