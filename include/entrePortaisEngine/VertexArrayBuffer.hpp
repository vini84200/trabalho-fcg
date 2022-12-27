#ifndef ENTREPORTAIS_VERTEXARRAYBUFFER_HPP
#define ENTREPORTAIS_VERTEXARRAYBUFFER_HPP

#include <vector>
#include <optional>
#include <memory>
#include "glad/glad.h"

namespace entre_portais {
    class VertexArrayBuffer;

    struct Attribute {
        GLuint index;
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        const GLvoid *offset;
    };

    class BufferBuilder {
    public:
        BufferBuilder();

        ~BufferBuilder();

        void
        addAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *offset);

        void setData(const void *data, GLsizeiptr size);

        void setUsage(GLenum usage);

        [[nodiscard]] unsigned int getID() const;

    private:
        const void *data_ = nullptr;
        GLsizeiptr size_ = 0;
        std::vector<Attribute> attributes_;
        unsigned int id_ = 0;
        GLenum usage_ = GL_STATIC_DRAW;

        friend class VertexArrayBuffer;

        friend class VBOBuffer;

        friend class EBOBuffer;
    };

    struct BoundAttribute {
        unsigned int index;
        unsigned int size;
        unsigned int type;
        bool normalized;
        unsigned int stride;
        unsigned int offset;
        unsigned int divisor;

        unsigned int vbo;
        unsigned int vao;
    } __attribute__((packed));

    class VBOBuffer {
    public:
        ~VBOBuffer();

        void setData(const void *data, unsigned int size);

        void setSubData(const void *data, unsigned int size, unsigned int offset);

        void bind() const;

        static void unbind();

        [[nodiscard]] unsigned int getId() const;

        [[nodiscard]] const std::vector<BoundAttribute> &getAttributes() const;

    private:
        VBOBuffer(BufferBuilder builder, unsigned int id, VertexArrayBuffer *vao);

        unsigned int id_ = 0;
        VertexArrayBuffer *vao_ = nullptr;
        std::vector<BoundAttribute> attributes_;

        friend class VertexArrayBuffer;
    };

    class EBOBuffer {
    public:
        ~EBOBuffer();

        void setData(const void *data, unsigned int size);

        void setSubData(const void *data, unsigned int size, unsigned int offset);

        void bind();

        void unbind();

        unsigned int getId() const;

    private:
        EBOBuffer(BufferBuilder builder, VertexArrayBuffer *vao);

        unsigned int id_ = 0;
        VertexArrayBuffer *vao_ = nullptr;

        friend class VertexArrayBuffer;
    };

    class VertexArrayBuffer {
    public:
        VertexArrayBuffer();

        ~VertexArrayBuffer();

        void Commit();

        bool isCommitted() const;

        bool IsClean() const;

        void addBufferToQueue(BufferBuilder *bufferBuilder);

        void addEBOToQueue(BufferBuilder *bufferBuilder);

        BoundAttribute *getAttribute(unsigned int location);

        void bind();

        void unbind();

        bool isBound() const;

        void setEBO(EBOBuffer *ebo);

    private:
        unsigned int id_ = 0;

        std::vector<VBOBuffer *> buffers_;
        EBOBuffer *ebo_ = nullptr;
        std::vector<BoundAttribute> attributes_;

        std::vector<BufferBuilder> buffersToBuild_;
        std::optional<BufferBuilder> eboToBuild_ = std::nullopt;

        static unsigned int boundVAO_;
    };
}


#endif //ENTREPORTAIS_VERTEXARRAYBUFFER_HPP