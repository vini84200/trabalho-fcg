//
// Created by vini84200 on 12/25/22.
//

#include <assert.h>
#include "entrePortaisEngine/VertexArrayBuffer.hpp"
#include "entrePortaisEngine/Logger.hpp"

namespace entre_portais {
    unsigned int VertexArrayBuffer::boundVAO_ = 0;

    VertexArrayBuffer::VertexArrayBuffer() {
        log("VertexArrayBuffer::VertexArrayBuffer()");
        glGenVertexArrays(1, &id_);
    }

    VertexArrayBuffer::~VertexArrayBuffer() {
        log("VertexArrayBuffer::~VertexArrayBuffer()");
        glDeleteVertexArrays(1, &id_);
        // Delete all VBOs
        for (auto &vbo: buffers_) {
            delete vbo;
        }
        // Delete the EBO
        if (ebo_) {
            delete ebo_;
        }
    }

    void VertexArrayBuffer::bind() {
        if (boundVAO_ != id_) {
            boundVAO_ = id_;
            glBindVertexArray(id_);
        }
    }

    void VertexArrayBuffer::unbind() {
        if (boundVAO_ != 0) {
            boundVAO_ = 0;
            glBindVertexArray(0);
        }
    }

    void VertexArrayBuffer::addBufferToQueue(BufferBuilder *bufferBuilder) {
        buffersToBuild_.push_back(*bufferBuilder);
    }

    void VertexArrayBuffer::addEBOToQueue(BufferBuilder *bufferBuilder) {
        eboToBuild_ = *bufferBuilder;
    }

    BoundAttribute *VertexArrayBuffer::getAttribute(unsigned int location) {
        for (auto &attribute: attributes_) {
            if (attribute.index == location) {
                return &attribute;
            }
        }
        return nullptr;
    }

    void VertexArrayBuffer::Commit() {
        bind();
        auto buffersToCreate = static_cast<GLsizei>(buffersToBuild_.size());
        if (buffersToCreate != 0) {

            auto ids = new unsigned int[buffersToCreate];
            glGenBuffers(buffersToCreate, ids);

            for (int i = 0; i < buffersToCreate; i++) {
                VBOBuffer *vboBuffer = new VBOBuffer(buffersToBuild_[i], ids[i], this);
                buffers_.push_back(vboBuffer);
                for (auto &attribute: vboBuffer->getAttributes()) {
                    attributes_.push_back(attribute);
                }
            }
            buffersToBuild_.clear();
            delete[] ids;
        }

        if (eboToBuild_.has_value()) {
            auto eboBuffer = new EBOBuffer(*eboToBuild_, this);
            eboToBuild_ = std::nullopt;
        }
        unbind();
    }

    bool VertexArrayBuffer::IsClean() const {
        return buffers_.empty() && ebo_ == 0;
    }

    bool VertexArrayBuffer::isCommitted() const {
        return buffersToBuild_.empty() && !eboToBuild_.has_value();
    }

    bool VertexArrayBuffer::isBound() const {
        return boundVAO_ == id_;
    }

    void VertexArrayBuffer::setEBO(EBOBuffer *ebo) {
        ebo_ = ebo;
    }

    BufferBuilder::BufferBuilder() = default;

    BufferBuilder::~BufferBuilder() = default;

    void BufferBuilder::addAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                     const GLvoid *offset) {
        Attribute a = {index, size, type, normalized, stride, offset};
        attributes_.push_back(a);
    }

    void BufferBuilder::setData(const void *data, GLsizeiptr size) {
        data_ = data;
        size_ = size;
    }

    void BufferBuilder::setUsage(GLenum usage) {
        usage_ = usage;
    }

    unsigned int BufferBuilder::getID() const {
        return id_;
    }

    VBOBuffer::VBOBuffer(BufferBuilder builder, unsigned int id, VertexArrayBuffer *vao) : id_(id) {
        // Assumes that the VAO is already bound
        assert(("VAO is not bound", vao->isBound()));
        // Assumes that the id is already generated
        assert(("ID is not generated", id != 0));

        log("VBOBuffer::VBOBuffer()");
        bind();
        if (builder.data_ != nullptr) {
            glBufferData(GL_ARRAY_BUFFER, builder.size_, builder.data_, builder.usage_);
        }

        for (auto &attribute: builder.attributes_) {
            glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized,
                                  attribute.stride,
                                  attribute.offset);
            glEnableVertexAttribArray(attribute.index);
        }
        unbind();
    }

    VBOBuffer::~VBOBuffer() {
        log("VBOBuffer::~VBOBuffer()");
        glDeleteBuffers(1, &id_);
    }

    void VBOBuffer::setData(const void *data, unsigned int size) {
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        unbind();
    }

    void VBOBuffer::setSubData(const void *data, unsigned int size, unsigned int offset) {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        unbind();
    }

    void VBOBuffer::bind() const {
        log("VBOBuffer::bind()");
        glBindBuffer(GL_ARRAY_BUFFER, id_);
    }

    void VBOBuffer::unbind() {
        log("VBOBuffer::unbind()");
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    unsigned int VBOBuffer::getId() const {
        return id_;
    }

    const std::vector<BoundAttribute> &VBOBuffer::getAttributes() const {
        return attributes_;
    }

    EBOBuffer::EBOBuffer(BufferBuilder builder, VertexArrayBuffer *vao) : vao_(vao) {
        log("EBOBuffer::EBOBuffer()");
        glGenBuffers(1, &id_);
        // Assumes that the vao is already bound
        assert(("VAO is not bound", vao_->isBound()));

        bind();
        if (builder.data_ != nullptr) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, builder.size_, builder.data_, builder.usage_);
        }
        vao_->setEBO(this);
    }

    EBOBuffer::~EBOBuffer() {
        log("EBOBuffer::~EBOBuffer()");
        glDeleteBuffers(1, &id_);
    }

    void EBOBuffer::setData(const void *data, unsigned int size) {
        if (vao_->isBound()) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        } else {
            vao_->bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
            vao_->unbind();
        }
    }

    void EBOBuffer::setSubData(const void *data, unsigned int size, unsigned int offset) {
        if (vao_->isBound()) {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
        } else {
            vao_->bind();
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
            vao_->unbind();
        }
    }

    void EBOBuffer::bind() {
        log("EBOBuffer::bind()");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

    void EBOBuffer::unbind() {
        log("EBOBuffer::unbind()");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    unsigned int EBOBuffer::getId() const {
        return id_;
    }
}