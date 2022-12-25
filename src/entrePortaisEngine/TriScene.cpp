
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

        float colors[] = {
                1.f, 0.f, 0.f, 1.f,
                0.f, 1.f, 0.f, 1.f,
                0.f, 0.f, 1.f, 1.f,
        };

        vao_ = std::make_shared<VAOBuffer>();
        BufferBuilder pos_b;
        pos_b.addAttribute(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        pos_b.setData(vertices, sizeof(vertices));
        pos_b.setUsage(GL_STATIC_DRAW);

        vao_->addBufferToQueue(&pos_b);

        BufferBuilder color_b;
        color_b.addAttribute(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
        color_b.setData(colors, sizeof(colors));
        color_b.setUsage(GL_STATIC_DRAW);

        vao_->addBufferToQueue(&color_b);

        vao_->Commit();


//
//        // INDICES
//        unsigned int indices[] = {
//                0, 1, 2
//        };
//
//        GLuint indices_id;
//        glGenBuffers(1, &indices_id);
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//        glBindVertexArray(0);

        // program_
        shader_ = new Shader("assets/shaders/tri.vert", "assets/shaders/tri.frag");

    }

    TriScene::~TriScene() {
    }

    void TriScene::update() {

    }

    void TriScene::render() {
        shader_->use();
        vao_->bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        vao_->unbind();
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