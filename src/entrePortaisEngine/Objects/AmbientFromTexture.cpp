#include "entrePortaisEngine/Objects/AmbientFromTexture.hpp"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/render/TextureManager.hpp"
#include "stb_image.h"

namespace entre_portais {

    void AmbientFromTexture::onKey(int key, int scancode, int action, int mods) {

    }

    void AmbientFromTexture::onMouseButton(int button, int action, int mods) {

    }

    void AmbientFromTexture::onExit() {

    }

    void AmbientFromTexture::update(float deltaTime) {
    }

    void AmbientFromTexture::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("skybox");
        submit(renderer, RenderPass::BACKGROUND);

        mesh_ = std::make_unique<AmbientMesh>(texturePath_);


        // Initialize VAO



    }

    void AmbientFromTexture::onResize(int width, int height) {
    }

    AmbientFromTexture::AmbientFromTexture(std::array<std::string, 6> facesImages) : IObject("Skybox"),
                                                                                     texturePath_(facesImages) {

    }

    void AmbientFromTexture::render(RenderPass current_pass) {
        if (!IsVisible()) return;
        if (current_pass == BACKGROUND) {
            mesh_->Draw(getShader(), current_pass);
        }
    }

    void AmbientMesh::Draw(Shader shaderInUse, RenderPass current_pass) {
        UseVAO();
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureid_);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        UnbindVAO();
    }

    AmbientMesh::AmbientMesh(std::array<std::string, 6> faceTextures) {

        // TODO: Remover gambiarra
        glGenTextures(1, reinterpret_cast<GLuint *>(&textureid_));
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureid_);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < 6; i++) {
            stbi_set_flip_vertically_on_load(false);
            unsigned char *data = stbi_load(faceTextures[i].c_str(), &width, &height, &nrChannels, 4);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            } else {
                spdlog::error("Cubemap texture failed to load at path: {}", faceTextures[i]);
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glad_glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        // Cube VAO
        const float size = 1.0f;
        auto vao = std::make_shared<VertexArrayBuffer>();
        BufferBuilder vboPos, ebo;
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
        unsigned int indices[] = {
                // Front
                0, 1, 2,
                0, 2, 3,
                // Left
                0, 3, 4,
                3, 5, 4,
                // Back
                6, 5, 7,
                6, 4, 5,
                // Right
                1, 6, 2,
                2, 6, 7,
                // Top
                0, 6, 1,
                6, 0, 4,
                // Bottom
                5, 2, 7,
                2, 5, 3
        };

        vboPos.addAttribute(POSITIONS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboPos.setData(positions, sizeof(float) * 3 * 8);
        vboPos.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboPos);

        ebo.setData(indices, sizeof(indices));
        ebo.setUsage(GL_STATIC_DRAW);
        vao->addEBOToQueue(&ebo);

        vao->Commit();

        SetVAO(std::move(vao), 12 * 3);
    }
} // entre_portais