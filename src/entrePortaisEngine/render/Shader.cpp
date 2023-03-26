#include "entrePortaisEngine/render/Shader.hpp"

#include <fstream>
#include <ios>
#include <sstream>

#include "entrePortaisEngine/Logger.hpp"

namespace entre_portais {
    Logger gShaderLogger("Shader");

    Shader::Shader(const char *vertexPath, const char *fragmentPath) : program_(glCreateProgram()) {
        gShaderLogger.getLogger()->debug("Shader::Shader - vertexPath: {}, fragmentPath: {}", vertexPath, fragmentPath);
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        LoadShader(vertexPath, vertexShader);
        LoadShader(fragmentPath, fragmentShader);

        glAttachShader(program_, vertexShader);
        glAttachShader(program_, fragmentShader);
        glLinkProgram(program_);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader() {
    }

    void Shader::use() {
        glUseProgram(program_);
    }

    void Shader::LoadShader(const char *filename, GLuint shader_id) {
        std::ifstream file;
        try {
            file.exceptions(std::ifstream::badbit);
            file.open(filename);
        }
        catch (const std::exception &e) {
            gShaderLogger.getLogger()->error("Shader::LoadShader - {}", e.what());
            throw e;
        }
        std::stringstream shader;
        shader << file.rdbuf();
        std::string str = shader.str();
        if (str.empty()) {
            gShaderLogger.getLogger()->error("Shader::LoadShader - Empty shader file: {}", filename);
            throw std::runtime_error("Empty shader file");
        }
        const GLchar *shader_string = str.c_str();
        const GLint shader_string_length = static_cast<GLint>(str.length());

        // Define o código do shader GLSL, contido na string "shader_string"
        glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

        // Compila o código do shader GLSL (em tempo de execução)
        glCompileShader(shader_id);

        // Verificamos se ocorreu algum erro ou "warning" durante a compilação
        GLint compiled_ok;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

        GLint log_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos memória para guardar o logBool de compilação.
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
                output += "== Start of compilation logBool\n";
                output += log;
                output += "== End of compilation logBool\n";
            } else {
                output += "WARNING: OpenGL compilation of \"";
                output += filename;
                output += "\".\n";
                output += "== Start of compilation logBool\n";
                output += log;
                output += "== End of compilation logBool\n";
            }

            fprintf(stderr, "%s", output.c_str());
        }

        // A chamada "delete" em C++ é equivalente ao "free()" do C
        delete[] log;
    }

    void Shader::setUniformInt(const char *name, int value) {
        glUniform1i(glGetUniformLocation(program_, name), value);
    }

    void Shader::setUniformBool(const char *name, bool value) {
        glUniform1i(glGetUniformLocation(program_, name), value);
    }

    void Shader::setUniformFloat(const char *name, float value) {
        glUniform1f(glGetUniformLocation(program_, name), value);
    }

    void Shader::setUniformVec3(const char *name, float x, float y, float z) {
        glUniform3f(glGetUniformLocation(program_, name), x, y, z);
    }

    void Shader::setUniformVec3(const char *name, const glm::vec3 &value) {
        glUniform3f(glGetUniformLocation(program_, name), value.x, value.y, value.z);
    }

    void Shader::setUniformMat4(const char *name, const float *value) {
        glUniformMatrix4fv(glGetUniformLocation(program_, name), 1, GL_FALSE, value);
    }

    void Shader::setUniformMat4(const char *name, const glm::mat4 &value) {
        glUniformMatrix4fv(glGetUniformLocation(program_, name), 1, GL_FALSE, &value[0][0]);
    }

    void Shader::setID(int id) {
        id_ = id;
    }

    int Shader::getID() const {
        return id_;
    }

    void Shader::deleteShader() {
        glDeleteProgram(program_);
        gShaderLogger.getLogger()->debug("Shader esta {} sendo removida da GPU", id_);
    }

    void Shader::UnbindShader() {
        glUseProgram(0);
    }

    void Shader::setUniformUInt(const char *name, unsigned int value) const {
        glUniform1ui(glGetUniformLocation(program_, name), value);

    }

    void Shader::setUniformFloatInArray(const char *name, int index, float value) {
        glUniform1f(getLocationInArray(name, index), value);
    }

    void Shader::setUniformVec3InArray(const char *name, int index, float x, float y, float z) {
        glUniform3f(getLocationInArray(name, index), x, y, z);
    }

    void Shader::setUniformVec3InArray(const char *name, int index, const glm::vec3 &value) {
        glUniform3f(getLocationInArray(name, index), value.x, value.y, value.z);
    }

    GLint Shader::getLocationInArray(const char *name, int index) {
        std::string nameString(name);
        // Find the array index in the string
        size_t arrayIndex = nameString.find('[');
        // If the array index is found, insert the index in the string
        if (arrayIndex != std::string::npos) {
            nameString.insert(arrayIndex + 1, std::to_string(index));
        }
            // If the array index is not found, append the index to the string
        else {
            nameString += '[' + std::to_string(index) + ']';
        }
        return glGetUniformLocation(program_, nameString.c_str());

    }
}  // namespace entre_portais