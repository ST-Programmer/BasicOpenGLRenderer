#pragma once

#include <string>
#include "shaderprogram.hpp"

class Texture
{
public:
    Texture(const std::string& path, const std::string& type);
    ~Texture();

    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth()
    {
        return m_Width;
    }
    inline int GetHeight()
    {
        return m_Height;
    }
    inline const std::string& GetType() const
    {
        return m_Type;
    }
    void TexUnit(const ShaderProgram& shaderProgram, const char* uniformName,
                 unsigned int unit) const;

private:
    std::string m_Type;
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BitsPerPixel;
};
