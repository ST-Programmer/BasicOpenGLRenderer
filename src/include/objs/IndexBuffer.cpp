#include "IndexBuffer.hpp"

#include "utils.h"
#include <cassert>
#include <glad/glad.h>
#include <vector>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count)
{
    assert(sizeof(unsigned int) == sizeof(GLuint));

    GL_CALL(glGenBuffers(1, &m_RendererID));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), data,
                         GL_DYNAMIC_DRAW));
}

IndexBuffer::IndexBuffer(std::vector<unsigned int>& indices) : m_Count(indices.size())
{
    assert(sizeof(unsigned int) == sizeof(GLuint));

    GL_CALL(glGenBuffers(1, &m_RendererID));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), indices.data(),
                         GL_DYNAMIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_RendererID));
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_RendererID(other.m_RendererID), m_Count(other.m_Count)
{
    other.m_RendererID = 0;
    other.m_Count = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if (this != &other)
    {
        GL_CALL(glDeleteBuffers(1, &m_RendererID));
        m_RendererID = other.m_RendererID;
        m_Count = other.m_Count;
        other.m_RendererID = 0;
        other.m_Count = 0;
    }
    return *this;
}

void IndexBuffer::Bind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}
void IndexBuffer::Unbind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
