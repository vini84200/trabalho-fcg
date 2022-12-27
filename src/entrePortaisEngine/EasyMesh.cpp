#include "entrePortaisEngine/EasyMesh.hpp"

namespace entre_portais {
    EasyMesh::EasyMesh(ManyVertices vertices, const char *vertexPath, const char *fragmentPath) {
        auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
        SetShader(shader);
        auto vao = std::make_unique<VertexArrayBuffer>();
        BufferBuilder vboPos;
        BufferBuilder vboColor;
        BufferBuilder ebo;
        vboPos.addAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(EasyVertex), (void *) offsetof(EasyVertex, x));
        vboPos.setData(vertices.vertices.data(), sizeof(EasyVertex) * vertices.vertices.size());
        vboPos.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboPos);

        vboColor.addAttribute(1, 3, GL_FLOAT, GL_FALSE, sizeof(EasyVertex), (void *) offsetof(EasyVertex, r));
        vboColor.setData(vertices.vertices.data(), sizeof(EasyVertex) * vertices.vertices.size());
        vboColor.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboColor);

        ebo.setData(vertices.indices.data(), sizeof(unsigned int) * vertices.indices.size());
        ebo.setUsage(GL_STATIC_DRAW);
        vao->addEBOToQueue(&ebo);

        vao->Commit();

        SetVAO(std::move(vao), vertices.indices.size());
    }

    void EasyMesh::Draw() {
        UseShader();
        UseVAO();
        glDrawElements(GL_TRIANGLES, GetNumVertices(), GL_UNSIGNED_INT, 0);
        UnbindVAO();
        UnbindShader();
    }
} // entre_portais