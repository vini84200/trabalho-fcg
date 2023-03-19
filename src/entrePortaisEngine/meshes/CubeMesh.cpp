#include "entrePortaisEngine/meshes/CubeMesh.hpp"
#include "entrePortaisEngine/render/Renderer.hpp"

namespace entre_portais {
    CubeMesh::CubeMesh(float size, glm::vec3 color) : IMesh() {
        auto vao = std::make_shared<VertexArrayBuffer>();
        BufferBuilder vboPos, vboColor, ebo;
        float s = size / 2;
        float b = -s;

        glm::vec3 positions[] = {
                {b, b, b},
                {s, b, b},
                {s, s, b},
                {b, s, b},
                {b, b, s},
                {b, s, s},
                {s, b, s},
                {s, s, s}
        };
        glm::vec3 colors[] = {
                color,
                color,
                color,
                color,
                color,
                color,
                color,
                color,
        };
        unsigned int indices[] = {
                // Front
                0, 2, 1,
                0, 3, 2,
                // Left
                0, 4, 3,
                3, 4, 5,
                // Back
                6, 7, 5,
                6, 5, 4,
                // Right
                1, 2, 6,
                2, 7, 6,
                // Top
                0, 1, 6,
                6, 4, 0,
                // Bottom
                5, 7, 2,
                2, 3, 5
        };

        vboPos.addAttribute(POSITIONS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboPos.setData(positions, sizeof(float) * 3 * 8);
        vboPos.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboPos);

        vboColor.addAttribute(COLORS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboColor.setData(colors, sizeof(float) * 3 * 8);
        vboColor.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboColor);

        ebo.setData(indices, sizeof(indices));
        ebo.setUsage(GL_STATIC_DRAW);
        vao->addEBOToQueue(&ebo);

        vao->Commit();

        SetVAO(std::move(vao), 12 * 3);
    }

    void CubeMesh::Draw(Shader shaderInUse, RenderPass current_pass) {
        UseVAO();
        glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);
        UnbindVAO();
    }
} // entre_portais