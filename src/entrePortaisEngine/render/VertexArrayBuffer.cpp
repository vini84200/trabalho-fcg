//
// Created by vini84200 on 12/25/22.
//

#include "entrePortaisEngine/render/VertexArrayBuffer.hpp"

#include <assert.h>

#include "entrePortaisEngine/Logger.hpp"

namespace entre_portais {
    Logger gLogger("VertexArrayBuffer");
    unsigned int VertexArrayBuffer::boundVAO_ = 0;

    VertexArrayBuffer::VertexArrayBuffer() {
        gLogger.getLogger()->debug("VertexArrayBuffer::VertexArrayBuffer()");
        glGenVertexArrays(1, &id_);
    }

    VertexArrayBuffer::~VertexArrayBuffer() {
        gLogger.getLogger()->debug("VertexArrayBuffer::~VertexArrayBuffer()");
        glDeleteVertexArrays(1, &id_);
        // Delete all VBOs
        for (auto &vbo: buffers_) {
            vbo.reset();
        }
        // Delete the EBO
        if (ebo_) {
            ebo_.reset();
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
        gLogger.getLogger()->debug("VertexArrayBuffer::Commit()");
        bind();
        auto buffersToCreate = buffersToBuild_.size();
        if (buffersToCreate != 0) {
            std::vector<unsigned int> ids(buffersToCreate);
            glGenBuffers(buffersToCreate, ids.data());

            for (int i = 0; i < buffersToCreate; i++) {
                auto vboBuffer = std::shared_ptr<VBOBuffer>(
                        new VBOBuffer(buffersToBuild_[i], ids[i], weak_from_this()));
                buffers_.push_back(vboBuffer);
                for (auto &attribute: vboBuffer->getAttributes()) {
                    attributes_.push_back(attribute);
                }
            }
            buffersToBuild_.clear();
        }

        if (eboToBuild_.has_value()) {
            auto eboBuffer = std::shared_ptr<EBOBuffer>(new EBOBuffer(eboToBuild_.value(), weak_from_this()));
            eboToBuild_ = std::nullopt;
            ebo_ = eboBuffer;
        }
        unbind();
    }

    bool VertexArrayBuffer::IsClean() const {
        return buffers_.empty() && ebo_.has_value();
    }

    bool VertexArrayBuffer::isCommitted() const {
        return buffersToBuild_.empty() && !eboToBuild_.has_value();
    }

    bool VertexArrayBuffer::isBound() const {
        return boundVAO_ == id_;
    }

    void VertexArrayBuffer::setEBO(std::shared_ptr<EBOBuffer> ebo) {
        ebo_ = ebo;
    }

    BufferBuilder::BufferBuilder() = default;

    BufferBuilder::~BufferBuilder() = default;

    void BufferBuilder::addAttribute(
            GLuint index,
            GLint size,
            GLenum type,
            GLboolean normalized,
            GLsizei stride,
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

    VBOBuffer::VBOBuffer(BufferBuilder builder, unsigned int id, std::weak_ptr<VertexArrayBuffer> vao) : id_(id) {
        // Assumes that the VAO is already bound
        assert(("VAO has expired", !vao.expired()));
        assert(("VAO is not bound", vao.lock()->isBound()));
        // Assumes that the id is already generated
        assert(("ID is not generated", id != 0));

        gLogger.getLogger()->debug("VBOBuffer::VBOBuffer()");
        bind();
        if (builder.data_ != nullptr) {
            glBufferData(GL_ARRAY_BUFFER, builder.size_, builder.data_, builder.usage_);
        }

        for (auto &attribute: builder.attributes_) {
            glVertexAttribPointer(
                    attribute.index, attribute.size, attribute.type, attribute.normalized, attribute.stride,
                    attribute.offset);
            glEnableVertexAttribArray(attribute.index);
        }
        unbind();
    }

    VBOBuffer::~VBOBuffer() {
        gLogger.getLogger()->debug("VBOBuffer::~VBOBuffer()");
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
        gLogger.getLogger()->debug("VBOBuffer::bind()");
        glBindBuffer(GL_ARRAY_BUFFER, id_);
    }

    void VBOBuffer::unbind() {
        gLogger.getLogger()->debug("VBOBuffer::unbind()");
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    unsigned int VBOBuffer::getId() const {
        return id_;
    }

    const std::vector<BoundAttribute> &VBOBuffer::getAttributes() const {
        return attributes_;
    }

    EBOBuffer::EBOBuffer(BufferBuilder builder, std::weak_ptr<VertexArrayBuffer> vao) : vao_(vao) {
        gLogger.getLogger()->debug("EBOBuffer::EBOBuffer()");
        glGenBuffers(1, &id_);
        assert(("ID is not generated", id_ != 0));
        // Assumes that the vao is already bound
        assert(("VAO is not bound", vao_.lock()->isBound()));

        bind();
        if (builder.data_ != nullptr) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, builder.size_, builder.data_, builder.usage_);
        }
    }

    EBOBuffer::~EBOBuffer() {
        gLogger.getLogger()->debug("EBOBuffer::~EBOBuffer()");
        glDeleteBuffers(1, &id_);
    }

    void EBOBuffer::setData(const void *data, unsigned int size) {
        auto vao = vao_.lock();
        if (vao->isBound()) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        } else {
            vao->bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
            vao->unbind();
        }
    }

    void EBOBuffer::setSubData(const void *data, unsigned int size, unsigned int offset) {
        auto vao = vao_.lock();
        if (vao->isBound()) {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
        } else {
            vao->bind();
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
            vao->unbind();
        }
    }

    void EBOBuffer::bind() {
        gLogger.getLogger()->debug("EBOBuffer::bind()");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

    void EBOBuffer::unbind() {
        gLogger.getLogger()->debug("EBOBuffer::unbind()");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    unsigned int EBOBuffer::getId() const {
        return id_;
    }
}  // namespace entre_portais