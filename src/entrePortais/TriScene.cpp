
#include <cstdio>
#include "entrePortais/TriScene.hpp"
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

        const char *vertex = "#version 330 core\n"
                             "layout (location = 0) in vec4 position;\n"
                             "layout (location = 1) in vec4 color;\n"
                             "out vec4 vertexColor;\n"
                             "void main()\n"
                             "{\n"
                             "   gl_Position = position;\n"
                             "   vertexColor = color;\n"
                             "}\0";

        const char *fragment = "#version 330 core\n"
                               "out vec4 FragColor;\n"
                               "in vec4 vertexColor;\n"
                               "void main()\n"
                               "{\n"
                               "   FragColor = vertexColor;\n"
                               "}\n\0";

        vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader_, 1, &vertex, NULL);
        glCompileShader(vertexShader_);
        GLint log_length = 0;
        glGetShaderiv(vertexShader_, GL_INFO_LOG_LENGTH, &log_length);
        GLchar *log = new GLchar[log_length];
        glGetShaderInfoLog(vertexShader_, log_length, &log_length, log);
        printf("Vertex Shader Log: (size %d) %s\n", log_length, log);
        fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader_, 1, &fragment, NULL);
        glCompileShader(fragmentShader_);
        glGetShaderiv(fragmentShader_, GL_INFO_LOG_LENGTH, &log_length);
        log = new GLchar[log_length];
        glGetShaderInfoLog(fragmentShader_, log_length, &log_length, log);
        printf("Fragment Shader Log: (size %d) %s\n", log_length, log);

        program_ = glCreateProgram();
        glAttachShader(program_, vertexShader_);
        glAttachShader(program_, fragmentShader_);
        glLinkProgram(program_);
        /*glDeleteShader(vertexShader_);
        glDeleteShader(fragmentShader_);*/

    }

    TriScene::~TriScene() {
    }

    void TriScene::update() {

    }

    void TriScene::render() {
        glUseProgram(program_);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
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