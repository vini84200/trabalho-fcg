#ifndef ENTREPORTAIS_SHADER_HPP
#define ENTREPORTAIS_SHADER_HPP

#include "glad/glad.h"

namespace entre_portais {

    class Shader {
    public:
        Shader(const char *vertexPath, const char *fragmentPath);

        ~Shader();

        // Remove os construtores e operadores de cópia e movimentação
        Shader(const Shader &other) = delete;

        Shader &operator=(const Shader &other) = delete;

        Shader(Shader &&other) = delete;

        Shader &operator=(Shader &&other) = delete;

        void use();

    private:
        void LoadShader(const char *filePath, GLuint shaderId);

        unsigned int vertexShader_, fragmentShader_, program_;
    };

} // entre_portais

#endif //ENTREPORTAIS_SHADER_HPP
