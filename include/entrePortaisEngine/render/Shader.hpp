#ifndef ENTREPORTAIS_SHADER_HPP
#define ENTREPORTAIS_SHADER_HPP

#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"

namespace entre_portais {
    class ShadersManager;

    class Shader {
    public:
        ~Shader();

        void use();

        void setUniformInt(const char *name, int value);

        void setUniformUInt(const char *name, unsigned int value) const;

        void setUniformFloat(const char *name, float value);

        void setUniformVec3(const char *name, float x, float y, float z);

        void setUniformVec3(const char *name, const glm::vec3 &value);

        void setUniformMat4(const char *name, const float *value);

        void setUniformMat4(const char *name, const glm::mat4 &value);

        GLint getLocationInArray(const char *name, int index);

        void setUniformFloatInArray(const char *name, int index, float value);

        void setID(int id);

        int getID() const;

        virtual void UnbindShader();

        void setUniformBool(const char *name, bool value);

        void setUniformVec3InArray(const char *name, int index, float x, float y, float z);

        void setUniformVec3InArray(const char *name, int index, const glm::vec3 &value);

    private:
        Shader(const char *vertexPath, const char *fragmentPath);

        void LoadShader(const char *filePath, GLuint shaderId);

        void deleteShader();

        unsigned int program_;

        int id_;

        friend class ShadersManager;

    };

} // entre_portais

#endif //ENTREPORTAIS_SHADER_HPP
