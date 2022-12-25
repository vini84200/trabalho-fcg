#include "entrePortaisEngine/Buffer.hpp"

#include <utility>

namespace entre_portais {
    VBOBuffer::VBOBuffer(int size, const void *data, GLenum usage) : size_(size), usage_(usage) {
        glGenBuffers(1, &id_);
        SetData(size, data, usage);

    }

    void VBOBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
    }

    void VBOBuffer::Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VBOBuffer::SetData(GLsizeiptr size, const void *data, GLenum usage) const {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        Unbind();
    }

    VBOBuffer::~VBOBuffer() {
        glDeleteBuffers(1, &id_);
    }

    void VBOBuffer::SetSubData(GLintptr offset, GLsizeiptr size, const void *data) const {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        Unbind();
    }

    VAOBuffer::VAOBuffer() {
        glGenVertexArrays(1, &id_);
    }

    VAOBuffer::~VAOBuffer() {
        glDeleteVertexArrays(1, &id_);
    }

    void VAOBuffer::Bind() {
        glBindVertexArray(id_);
    }

    void VAOBuffer::Unbind() {
        glBindVertexArray(0);
    }

    std::shared_ptr<BoundAttribute> VAOBuffer::AddAttribute(Attribute attribute, VBOBuffer *vbo) {
        Bind();
        vbo->Bind();
        attribute.SetVertexAttribPointer();
        Unbind();
        vbo->Unbind();
        std::shared_ptr<BoundAttribute> ptr(new BoundAttribute(attribute, std::shared_ptr<VBOBuffer>(vbo)));
        attributes_.push_back(ptr);
        ptr->Enable();
        return ptr;
    }

    void VAOBuffer::EnableAttributes() {
        for (auto &attribute: attributes_) {
            attribute->Enable();
        }
    }

    void VAOBuffer::DisableAttributes() {
        for (auto &attribute: attributes_) {
            attribute->Disable();
        }
    }

    void VAOBuffer::AddEBO(EBOBuffer *ebo) {
        ebo_ = ebo;
        Bind();
        ebo->Bind();
        Unbind();
        ebo->Unbind();
        ebo->vao_ = this;
    }

    void VAOBuffer::DrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
        Bind();
        ebo_->Bind();
        glDrawElements(mode, count, type, indices);
        ebo_->Unbind();
    }

    EBOBuffer::EBOBuffer(int size, const void *data, GLenum usage) : size_(size), usage_(usage) {
        glGenBuffers(1, &id_);
        SetData(size, data, usage);
    }

    EBOBuffer::~EBOBuffer() {
        glDeleteBuffers(1, &id_);
    }

    void EBOBuffer::SetData(GLsizeiptr size, const void *data, GLenum usage) {
        vao_->Bind();
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
        vao_->Unbind();
    }

    void EBOBuffer::Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

    void EBOBuffer::Unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Attribute::Attribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                         const void *pointer)
            : index_(index), size_(size), type_(type), normalized_(normalized), stride_(stride), pointer_(pointer) {
    }

    void Attribute::SetVertexAttribPointer() {
        glVertexAttribPointer(index_, size_, type_, normalized_, stride_, pointer_);
    }

    BoundAttribute::BoundAttribute(Attribute attribute, std::shared_ptr<VBOBuffer> vbo)
            : attribute_(attribute), vbo_(std::move(vbo)) {

    }

    void BoundAttribute::Enable() {
        if (!enabled_) {
            // Bind the VAO and VBO
            vao_->Bind();
            vbo_->Bind();
            glEnableVertexAttribArray(attribute_.index_);
            enabled_ = true;
            vao_->Unbind();
            vbo_->Unbind();
        }
    }

    void BoundAttribute::Disable() {
        if (enabled_) {
            vao_->Bind();
            vbo_->Bind();
            glDisableVertexAttribArray(attribute_.index_);
            enabled_ = false;
            vao_->Unbind();
            vbo_->Unbind();
        }
    }

    void BoundAttribute::Unbind() {
        if (enabled_) {
            Disable();
        }
    }
} // entre_portais