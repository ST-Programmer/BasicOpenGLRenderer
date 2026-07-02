#include "texture.hpp"

#include "shaderprogram.hpp"
#include "utils.h"
#include <glad/glad.h>
#include <stb/stb_image.h>

Texture::Texture(const std::string& path, const std::string& type)
    : m_Type(type), m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0),
      m_Height(0), m_BitsPerPixel(0)
{
    stbi_set_flip_vertically_on_load(true);
    m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4);

    if (!m_LocalBuffer)
    {
        ERR("Failed To Load Texture: " + m_FilePath);
        return;
    }

    GL_CALL(glGenTextures(1, &m_RendererID));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         m_LocalBuffer));
    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    Unbind();

    if (m_LocalBuffer)
    {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::Texture(Texture&& other) noexcept
    : m_Type(std::move(other.m_Type)), m_RendererID(other.m_RendererID),
      m_FilePath(std::move(other.m_FilePath)), m_LocalBuffer(other.m_LocalBuffer),
      m_Width(other.m_Width), m_Height(other.m_Height), m_BitsPerPixel(other.m_BitsPerPixel)
{
    other.m_RendererID = 0;
    other.m_LocalBuffer = nullptr;
    other.m_Width = 0;
    other.m_Height = 0;
    other.m_BitsPerPixel = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        GL_CALL(glDeleteTextures(1, &m_RendererID));

        m_Type = std::move(other.m_Type);
        m_RendererID = other.m_RendererID;
        m_FilePath = std::move(other.m_FilePath);
        m_LocalBuffer = other.m_LocalBuffer;
        m_Width = other.m_Width;
        m_Height = other.m_Height;
        m_BitsPerPixel = other.m_BitsPerPixel;

        other.m_RendererID = 0;
        other.m_LocalBuffer = nullptr;
        other.m_Width = 0;
        other.m_Height = 0;
        other.m_BitsPerPixel = 0;
    }
    return *this;
}

Texture::~Texture()
{
    GL_CALL(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::TexUnit(const ShaderProgram& shaderProgram, const char* uniformName,
                      unsigned int unit) const
{
    shaderProgram.SetInt(uniformName, unit);
}
