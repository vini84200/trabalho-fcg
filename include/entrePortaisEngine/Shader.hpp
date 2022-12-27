#ifndef ENTREPORTAIS_SHADER_HPP
#define ENTREPORTAIS_SHADER_HPP

#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"

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

        void setUniformInt(const char *name, int value);

        void setUniformFloat(const char *name, float value);

        void setUniformVec3(const char *name, float x, float y, float z);

        void setUniformVec3(const char *name, const glm::vec3 &value);

        void setUniformMat4(const char *name, const float *value);

        void setUniformMat4(const char *name, const glm::mat4 &value);

    private:
        void LoadShader(const char *filePath, GLuint shaderId);

        unsigned int vertexShader_, fragmentShader_, program_;
    };

} // entre_portais

#endif //ENTREPORTAIS_SHADER_HPP
