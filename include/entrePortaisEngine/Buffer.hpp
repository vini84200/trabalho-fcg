#ifndef ENTREPORTAIS_BUFFER_HPP
#define ENTREPORTAIS_BUFFER_HPP

#include <vector>
#include <memory>
#include "glad/glad.h"

namespace entre_portais {
    class EBOBuffer;

    class VAOBuffer;

    class BoundAttribute;

    class VBOBuffer {
    public:
        VBOBuffer(int size, const void *data, GLenum usage);

        ~VBOBuffer();

        void SetData(GLsizeiptr size, const void *data, GLenum usage) const;

        void SetSubData(GLintptr offset, GLsizeiptr size, const void *data) const;

        void Bind() const;

        static void Unbind();

    private:
        unsigned int id_;
        int size_;
        GLenum usage_;
    };

    class Attribute {
    public:
        Attribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

        void SetVertexAttribPointer();

    private:
        GLuint index_;
        GLint size_;
        GLenum type_;
        GLboolean normalized_;
        GLsizei stride_;
        const void *pointer_;

        friend class BoundAttribute;
    };

    class BoundAttribute {
    public:
        void Enable();

        void Disable();

        void Unbind();

    private:
        BoundAttribute(Attribute attribute, std::shared_ptr<VBOBuffer> vbo);

        Attribute attribute_;
        const std::shared_ptr<VBOBuffer> vbo_;
        bool enabled_;
        const std::shared_ptr<VAOBuffer> vao_;

        friend class VAOBuffer;
    };

    class VAOBuffer {
    public:
        VAOBuffer();

        ~VAOBuffer();

        void Bind();

        void Unbind();

        std::shared_ptr<BoundAttribute> AddAttribute(Attribute attribute, VBOBuffer *vbo);

        void EnableAttributes();

        void DisableAttributes();

        void AddEBO(EBOBuffer *ebo);

        void DrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);

    private:
        unsigned int id_{0};
        std::vector<std::shared_ptr<BoundAttribute>> attributes_;
        EBOBuffer *ebo_;
    };

    class EBOBuffer {
    public:
        EBOBuffer(int size, const void *data, GLenum usage);

        ~EBOBuffer();

        void SetData(GLsizeiptr size, const void *data, GLenum usage);

        void SetSubData(GLintptr offset, GLsizeiptr size, const void *data);

        void Bind();

        void Unbind();

    private:
        unsigned int id_;
        int size_;
        GLenum usage_;
        VAOBuffer *vao_;

        friend class VAOBuffer;
    };
} // entre_portais

#endif //ENTREPORTAIS_BUFFER_HPP
