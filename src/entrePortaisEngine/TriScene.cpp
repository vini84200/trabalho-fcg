
#include <cstdio>
#include "entrePortaisEngine/TriScene.hpp"
#include "glad/glad.h"

namespace entre_portais {
    TriScene::TriScene() {
    }

    void TriScene::initialize() {
        printf("Initializing TriScene\n");
        // Draw a square
        float vertices[] = {
                0.5f, 0.5f, 0.0f, 1.0,
                0.5f, -0.5f, 0.0f, 1.0,
                -0.5f, -0.5f, 0.0f, 1.0,
                -0.5f, 0.5f, 0.0f, 1.0,

        };

        float colors[] = {
                1.f, 0.f, 0.f, 1.f,
                0.f, 1.f, 0.f, 1.f,
                0.f, 0.f, 1.f, 1.f,
                1.f, 0.f, 1.f, 1.f,
        };

        vao_ = std::make_shared<VAOBuffer>();
        // BUFFER POSITIONS
        BufferBuilder pos_b;
        pos_b.addAttribute(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        pos_b.setData(vertices, sizeof(vertices));
        pos_b.setUsage(GL_STATIC_DRAW);

        vao_->addBufferToQueue(&pos_b);

        // BUFFER COLORS
        BufferBuilder color_b;
        color_b.addAttribute(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
        color_b.setData(colors, sizeof(colors));
        color_b.setUsage(GL_STATIC_DRAW);

        vao_->addBufferToQueue(&color_b);

        // INDICES
        unsigned int indices[] = {
                0, 1, 3,
                1, 2, 3
        };
        BufferBuilder indices_b;
        indices_b.setData(indices, sizeof(indices));
        indices_b.setUsage(GL_STATIC_DRAW);
        vao_->addEBOToQueue(&indices_b);

        vao_->Commit();



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
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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