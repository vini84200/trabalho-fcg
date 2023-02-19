//
// Created by vini84200 on 12/26/22.
//

#ifndef ENTREPORTAIS_IMESH_HPP
#define ENTREPORTAIS_IMESH_HPP


#include <memory>
#include <utility>
#include "entrePortaisEngine/render/VertexArrayBuffer.hpp"
#include "entrePortaisEngine/render/Shader.hpp"
#include "entrePortaisEngine/render/ShadersManager.hpp"

namespace entre_portais {

    class IMesh {
    public:
        IMesh(std::string shaderName);

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

        virtual void SetShader(Shader shader) {
            shader_ = shader;
        };

        virtual void SetShader(std::string name) {
            auto sm = ShadersManager::getInstance();
            shader_ = sm->getShader(name);
        };

        virtual Shader *GetShader() {
            return &shader_.value();
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

    public:
        Shader getShader() const {
            return shader_.value();
        }


    private:
        std::optional<Shader> shader_;
        unsigned int vertexCount_;
        std::shared_ptr<VertexArrayBuffer> vertexArray_;
    };


} // entre_portais

#endif //ENTREPORTAIS_IMESH_HPP
