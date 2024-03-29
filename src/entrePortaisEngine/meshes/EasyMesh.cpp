#include "entrePortaisEngine/meshes/EasyMesh.hpp"
#include "entrePortaisEngine/render/Renderer.hpp"

namespace entre_portais {
    EasyMesh::EasyMesh(ManyVertices vertices, std::string shaderName) : IMesh() {
        auto vao = std::make_shared<VertexArrayBuffer>();
        BufferBuilder vboPos;
        BufferBuilder vboColor;
        BufferBuilder ebo;
        vboPos.addAttribute(POSITIONS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(EasyVertex),
                            (void *) offsetof(EasyVertex, x));
        vboPos.setData(vertices.vertices.data(), sizeof(EasyVertex) * vertices.vertices.size());
        vboPos.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboPos);

        vboColor.addAttribute(COLORS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(EasyVertex),
                              (void *) offsetof(EasyVertex, r));
        vboColor.setData(vertices.vertices.data(), sizeof(EasyVertex) * vertices.vertices.size());
        vboColor.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboColor);

        ebo.setData(vertices.indices.data(), sizeof(unsigned int) * vertices.indices.size());
        ebo.setUsage(GL_STATIC_DRAW);
        vao->addEBOToQueue(&ebo);

        vao->Commit();

        SetVAO(std::move(vao), vertices.indices.size());
    }

    void EasyMesh::Draw(Shader shaderInUse, RenderPass current_pass) {
        UseVAO();
        glDrawElements(GL_TRIANGLES, GetNumVertices(), GL_UNSIGNED_INT, 0);
        UnbindVAO();
    }
}  // namespace entre_portais