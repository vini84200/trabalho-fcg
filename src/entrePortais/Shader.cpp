//
// Created by vini84200 on 12/22/22.
//

#include <ios>
#include <fstream>
#include <sstream>
#include "entrePortais/Shader.hpp"

namespace entre_portais {
    Shader::Shader(const char *vertexPath, const char *fragmentPath) {
        vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
        program_ = glCreateProgram();

        LoadShader(vertexPath, vertexShader_);
        LoadShader(fragmentPath, fragmentShader_);

        glAttachShader(program_, vertexShader_);
        glAttachShader(program_, fragmentShader_);
        glLinkProgram(program_);
        glDeleteShader(vertexShader_);
        glDeleteShader(fragmentShader_);
    }

    Shader::~Shader() {

    }

    void Shader::use() {
        glUseProgram(program_);
    }

    void Shader::LoadShader(const char *filename, GLuint shader_id) {
        std::ifstream file;
        try {
            file.exceptions(std::ifstream::failbit);
            file.open(filename);
        } catch (std::exception &e) {
            fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
            std::exit(EXIT_FAILURE);
        }
        std::stringstream shader;
        shader << file.rdbuf();
        std::string str = shader.str();
        const GLchar *shader_string = str.c_str();
        const GLint shader_string_length = static_cast<GLint>( str.length());

        // Define o código do shader GLSL, contido na string "shader_string"
        glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

        // Compila o código do shader GLSL (em tempo de execução)
        glCompileShader(shader_id);

        // Verificamos se ocorreu algum erro ou "warning" durante a compilação
        GLint compiled_ok;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

        GLint log_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos memória para guardar o log de compilação.
        // A chamada "new" em C++ é equivalente ao "malloc()" do C.
        GLchar *log = new GLchar[log_length];
        glGetShaderInfoLog(shader_id, log_length, &log_length, log);

        // Imprime qualquer erro no terminal ou "warning" de compilação
        if (log_length != 0) {
            std::string output;

            if (!compiled_ok) {
                output += "ERROR: OpenGL compilation of \"";
                output += filename;
                output += "\" failed.\n";
                output += "== Start of compilation log\n";
                output += log;
                output += "== End of compilation log\n";
            } else {
                output += "WARNING: OpenGL compilation of \"";
                output += filename;
                output += "\".\n";
                output += "== Start of compilation log\n";
                output += log;
                output += "== End of compilation log\n";
            }

            fprintf(stderr, "%s", output.c_str());
        }

        // A chamada "delete" em C++ é equivalente ao "free()" do C
        delete[] log;

    }
} // entre_portais