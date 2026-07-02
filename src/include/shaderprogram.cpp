#include "shaderprogram.hpp"

#include "utils.h"

ShaderProgram::ShaderProgram(const char *vertexPath, const char *fragmentPath)
{
    CompileShaders(vertexPath, fragmentPath);
}

ShaderProgram::~ShaderProgram() { GL_CALL(glDeleteProgram(m_ID)); }

void ShaderProgram::CompileShaders(const char *vertexPath,
                                   const char *fragmentPath)
{
    m_ID = glCreateProgram();

    std::string vertexSource = ReadFile(vertexPath);
    std::string fragmentSource = ReadFile(fragmentPath);

    const char *vertexSourceC = vertexSource.c_str();
    const char *fragmentSourceC = fragmentSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GL_CALL(glShaderSource(vertexShader, 1, &vertexSourceC, NULL));
    GL_CALL(glCompileShader(vertexShader));

    CheckShaderCompilation(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GL_CALL(glShaderSource(fragmentShader, 1, &fragmentSourceC, NULL));
    GL_CALL(glCompileShader(fragmentShader));

    CheckShaderCompilation(fragmentShader);

    GL_CALL(glAttachShader(m_ID, vertexShader));
    GL_CALL(glAttachShader(m_ID, fragmentShader));
    GL_CALL(glLinkProgram(m_ID));

    GL_CALL(glDeleteShader(vertexShader));
    GL_CALL(glDeleteShader(fragmentShader));
}

int ShaderProgram::GetUniformLocation(const char *name) const
{
    auto it = m_UniformLocationCache.find(name);
    if (it != m_UniformLocationCache.end())
    {
        return it->second;
    }

    int location = GL_CALL((glGetUniformLocation(m_ID, name)));
    m_UniformLocationCache.emplace(name, location);
    return location;
}

void ShaderProgram::CheckShaderCompilation(unsigned int shader) const
{
    int success;
    GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));

    if (!success)
    {
        int logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char *infolog = (char *)malloc(logLength);
        GL_CALL(glGetShaderInfoLog(shader, logLength, NULL, infolog));
        std::cerr << (std::string("Failed to Compile Shader: ") + infolog)
                  << std::endl;
    }
}