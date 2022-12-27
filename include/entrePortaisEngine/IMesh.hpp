//
// Created by vini84200 on 12/26/22.
//

#ifndef ENTREPORTAIS_IMESH_HPP
#define ENTREPORTAIS_IMESH_HPP


#include <memory>
#include <utility>
#include "VertexArrayBuffer.hpp"
#include "Shader.hpp"

namespace entre_portais {

    class IMesh {
    public:
        IMesh() = default;

        virtual ~IMesh() = default;

        IMesh(const IMesh &other) = delete;

        IMesh &operator=(const IMesh &other) = delete;

        IMesh(IMesh &&other) = delete;

        IMesh &operator=(IMesh &&other) = delete;

        virtual void Draw() = 0;

        virtual void SetVAO(std::shared_ptr<VertexArrayBuffer> vao, unsigned int numVertices = 0) {
            vertexArray_ = std::move(vao);
            vertexCount_ = numVertices;
        }

        virtual void SetShader(std::shared_ptr<Shader> shader) {
            shader_ = std::move(shader);
        };

        virtual void SetShader(const char *vertexPath, const char *fragmentPath) {
            shader_ = std::make_shared<Shader>(vertexPath, fragmentPath);
        };

        virtual Shader *GetShader() {
            return shader_.get();
        };

        virtual void UseShader() {
            shader_->use();
        }

        virtual void UseVAO() {
            vertexArray_->bind();
        }

        virtual void UnbindVAO() {
            vertexArray_->unbind();
        }

        virtual void UnbindShader() {
            glUseProgram(0);
        }

        virtual int GetNumVertices() {
            return vertexCount_;
        }

    private:
        std::shared_ptr<Shader> shader_;
        unsigned int vertexCount_;
        std::shared_ptr<VertexArrayBuffer> vertexArray_;
    };

} // entre_portais

#endif //ENTREPORTAIS_IMESH_HPP
