
#include <cstdio>
#include "entrePortaisEngine/TriScene.hpp"
#include "glad/glad.h"

namespace entre_portais {
    TriScene::TriScene() {
    }

    void TriScene::initialize() {
        printf("Initializing TriScene\n");
        float vertices[] = {
                -0.5f, -0.5f, 0.0f, 1.f,
                0.5f, -0.5f, 0.0f, 1.f,
                0.0f, 0.5f, 0.0f, 1.f,
        };
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glGenVertexArrays(1, &VAO);

        glad_glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint location = 0;
        GLint size = 4;
        glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        float colors[] = {
                1.f, 0.f, 0.f, 1.f,
                0.f, 1.f, 0.f, 1.f,
                0.f, 0.f, 1.f, 1.f,
        };

        glGenBuffers(1, &VBO_COLOR);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_COLOR);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        location = 1;
        size = 4;
        glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        // INDICES
        unsigned int indices[] = {
                0, 1, 2
        };

        GLuint indices_id;
        glGenBuffers(1, &indices_id);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glBindVertexArray(0);

        // program_
        shader_ = new Shader("assets/shaders/tri.vert", "assets/shaders/tri.frag");

    }

    TriScene::~TriScene() {
    }

    void TriScene::update() {

    }

    void TriScene::render() {
        shader_->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void TriScene::onExit() {

    }

    void TriScene::onKey(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {

    }

    void TriScene::onResize(int /*width*/, int /*height*/) {

    }

    void TriScene::onMouseButton(int /*button*/, int /*action*/, int /*mods*/) {

    }
} // entre_portais