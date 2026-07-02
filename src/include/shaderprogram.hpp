#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

#include "utils.h"

class ShaderProgram
{
  public:
    ShaderProgram(const char *vertexPath, const char *fragmentPath);

    ~ShaderProgram();

    void CompileShaders(const char *vertexPath, const char *fragmentPath);

    inline unsigned int getProgramID() const { return m_ID; }
    inline void Use() const { GL_CALL(glUseProgram(m_ID)); }

    inline void SetBool(const char *name, bool value) const
    {
        SetInt(name, (int)value);
    }
    inline void SetInt(const char *name, int value) const
    {
        Use();
        GL_CALL(glUniform1i(GetUniformLocation(name), value));
    }
    inline void SetFloat(const char *name, float value) const
    {
        Use();
        GL_CALL(glUniform1f(GetUniformLocation(name), value));
    }
    inline void SetVec3(const char *name, const glm::vec3 &value) const
    {
        SetVec3(name, glm::value_ptr(value));
    }
    inline void SetVec3(const char *name, const float *value) const
    {
        Use();
        GL_CALL(glUniform3fv(GetUniformLocation(name), 1, value));
    }
    inline void SetMat3(const char *name, const glm::mat3 &value) const
    {
        SetMat3(name, glm::value_ptr(value));
    }
    inline void SetMat4(const char *name, const glm::mat4 &value) const
    {
        SetMat4(name, glm::value_ptr(value));
    }
    inline void SetMat3(const char *name, const float *ptr) const
    {
        Use();
        GL_CALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, ptr));
    }
    inline void SetMat4(const char *name, const float *ptr) const
    {
        Use();
        GL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, ptr));
    }

  private:
    unsigned int m_ID;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;

    int GetUniformLocation(const char *name) const;

    void CheckShaderCompilation(unsigned int shader) const;
};