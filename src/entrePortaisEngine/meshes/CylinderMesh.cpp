#include "entrePortaisEngine/meshes/CylinderMesh.hpp"
#include "entrePortaisEngine/render/Renderer.hpp"

# define M_PIf        3.14159265358979323846f    /* pi */

namespace entre_portais {
    void CylinderMesh::Draw(Shader shaderInUse, RenderPass current_pass) {
        UseVAO();
        glDrawElements(GL_TRIANGLES, GetNumVertices(), GL_UNSIGNED_INT, 0);
        UnbindVAO();
    }

    CylinderMesh::CylinderMesh(float radius, float height, int edges, glm::vec3 color) : IMesh() {
        auto vao = std::make_shared<VertexArrayBuffer>();
        BufferBuilder vboPos, vboColor, ebo;

        glm::vec3 positions[edges * 2 + 2];
        glm::vec3 colors[edges * 2 + 2];

        float step = 2 * M_PIf / static_cast<float> (edges);
        for (int i = 0; i < edges; i++) {
            float angle = step * i;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            positions[i] = {x, -height / 2, z};
            positions[i + edges] = {x, height / 2, z};
        }
        positions[edges * 2] = {0, -height / 2, 0};
        positions[edges * 2 + 1] = {0, height / 2, 0};

        for (int i = 0; i < edges * 2 + 2; i++) {
            colors[i] = color;
        }

        unsigned int indices[edges * 12];

        // Lower face
        for (int i = 0; i < edges; i++) {
            indices[i * 3] = edges * 2;
            indices[i * 3 + 1] = i;
            indices[i * 3 + 2] = (i + 1) % edges;
        }
        // Upper face
        for (int i = 0; i < edges; i++) {
            indices[edges * 3 + i * 3] = edges * 2 + 1;
            indices[edges * 3 + i * 3 + 1] = edges + (i + 1) % edges;
            indices[edges * 3 + i * 3 + 2] = edges + i;
        }
        // Sides
        for (int i = 0; i < edges; i++) {
            indices[edges * 6 + i * 6] = i;
            indices[edges * 6 + i * 6 + 1] = edges + i;
            indices[edges * 6 + i * 6 + 2] = edges + (i + 1) % edges;
            indices[edges * 6 + i * 6 + 3] = i;
            indices[edges * 6 + i * 6 + 4] = edges + (i + 1) % edges;
            indices[edges * 6 + i * 6 + 5] = (i + 1) % edges;
        }

        vboPos.addAttribute(POSITIONS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboPos.setData(positions, sizeof(float) * 3 * (edges * 2 + 2));
        vboPos.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboPos);

        vboColor.addAttribute(COLORS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboColor.setData(colors, sizeof(float) * 3 * (edges * 2 + 2));
        vboColor.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboColor);

        ebo.setData(indices, sizeof(indices));
        ebo.setUsage(GL_STATIC_DRAW);
        vao->addEBOToQueue(&ebo);

        vao->Commit();

        SetVAO(std::move(vao), sizeof(indices) / sizeof(unsigned int));
    }
} // entre_portais