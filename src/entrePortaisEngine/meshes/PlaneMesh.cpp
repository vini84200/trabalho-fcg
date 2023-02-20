#include "entrePortaisEngine/meshes/PlaneMesh.hpp"
#include "utils/matrices.h"
#include "glm/gtc/type_ptr.hpp"

namespace entre_portais {
    void PlaneMesh::Draw() {
        UseVAO();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Draw Segments with GL_LINES
        glDrawElements(GL_LINES, 8 * (widthSegments_ * heightSegments_), GL_UNSIGNED_INT, (void *) (6 * sizeof(uint)));

        UnbindVAO();
    }

    PlaneMesh::PlaneMesh(float width, float height, int widthSegments, int heightSegments)
            : width_(width), height_(height), widthSegments_(widthSegments), heightSegments_(heightSegments) {
        auto vao = std::make_shared<VertexArrayBuffer>();
        BufferBuilder vboPos, vboColor, ebo;
        float w = width / 2;
        float h = height / 2;

        // Plane
        std::vector<glm::vec3> positions({
                                                 {-w, 0, -h},
                                                 {w,  0, -h},
                                                 {w,  0, h},
                                                 {-w, 0, h}
                                         });

        std::vector<glm::vec3> colors({
                                              {1, 0, 0},
                                              {0, 1, 0},
                                              {0, 0, 1},
                                              {1, 1, 1}
                                      });

        std::vector<uint> indices({
                                          2, 1, 0,
                                          0, 3, 2
                                  });

        // Segments
        float xStep = width / widthSegments;
        float zStep = height / heightSegments;
        glm::vec3 lineColor = {0, 0, 0};

        uint offset = 4;
        // FIXME: This is not the best way to do this
        // Tip: Use a single line to draw the segments
        // instead of drawing a small part of the line for each square
        // Think of drawing a game of tic-tac-toe
        for (int i = 0; i < widthSegments; i++) {
            for (int j = 0; j < heightSegments; j++) {
                float x = -w + xStep * i;
                float z = -h + zStep * j;
                // Draw the lines for a square
                positions.insert(positions.end(), {
                        {x,         0, z},
                        {x + xStep, 0, z},
                        {x + xStep, 0, z + zStep},
                        {x,         0, z + zStep},
                        {x,         0, z}
                });
                colors.insert(colors.end(), {
                        lineColor,
                        lineColor,
                        lineColor,
                        lineColor,
                        lineColor
                });

                // Draw the lines for the square
                indices.insert(indices.end(),
                               {
                                       offset, offset + 1,
                                       offset + 1, offset + 2,
                                       offset + 2, offset + 3,
                                       offset + 3, offset + 4
                               }
                );
                offset += 5;
            }
        }

        vboPos.addAttribute(POSITIONS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboPos.setData(positions.data(), positions.size() * sizeof(glm::vec3));
        vboPos.setUsage(GL_STATIC_DRAW);

        vao->addBufferToQueue(&vboPos);

        vboColor.addAttribute(COLORS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboColor.setData(colors.data(), colors.size() * sizeof(glm::vec3));
        vboColor.setUsage(GL_STATIC_DRAW);

        vao->addBufferToQueue(&vboColor);

        ebo.setData(indices.data(), indices.size() * sizeof(uint));
        ebo.setUsage(GL_STATIC_DRAW);

        vao->addEBOToQueue(&ebo);

        vao->Commit();

        SetVAO(std::move(vao), 6);
    }

    PlaneMesh::PlaneMesh(float width, float height, int segments)
            : PlaneMesh(width, height, segments, segments) {

    }

    PlaneMesh::PlaneMesh(float width, float height)
            : PlaneMesh(width, height, 1, 1) {

    }
} // entre_portais