#ifndef ENTREPORTAIS_IMESH_HPP
#define ENTREPORTAIS_IMESH_HPP


#include <memory>
#include <utility>
#include "entrePortaisEngine/render/VertexArrayBuffer.hpp"
#include "entrePortaisEngine/render/Shader.hpp"
#include "entrePortaisEngine/render/ShadersManager.hpp"
#include "imgui.h"
#include "entrePortaisEngine/render/Renderer.hpp"

namespace entre_portais {

#define POSITIONS_LOCATION 0
#define COLORS_LOCATION 1
#define NORMALS_LOCATION 2
#define TEXTURE_COORDINATES_LOCATION 3

    class IMesh {
    public:
        IMesh();

        virtual ~IMesh() = default;

        IMesh(const IMesh &other) = delete;

        IMesh &operator=(const IMesh &other) = delete;

        IMesh(IMesh &&other) = delete;

        IMesh &operator=(IMesh &&other) = delete;

        virtual void Draw(Shader shaderInUse, RenderPass current_pass) = 0;

        virtual void SetVAO(std::shared_ptr<VertexArrayBuffer> vao, unsigned int numVertices = 0) {
            vertexArray_ = std::move(vao);
            vertexCount_ = numVertices;
        }

        virtual void UseVAO() {
            vertexArray_->bind();
        }

        virtual void UnbindVAO() {
            vertexArray_->unbind();
        }

        virtual void RenderImGui(bool *p_open = nullptr) {
            ImGui::Text("Mesh not implemented");
        };


        virtual int GetNumVertices() {
            return vertexCount_;
        }

    private:
        unsigned int vertexCount_;
        std::shared_ptr<VertexArrayBuffer> vertexArray_;
    };


} // entre_portais

#endif //ENTREPORTAIS_IMESH_HPP
