#include "VertexBuffer.hpp"

#include "objs/Vertex.hpp"
#include "utils.h"
#include <glad/glad.h>
#include <vector>

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    GL_CALL(glGenBuffers(1, &m_RendererID));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::VertexBuffer(std::vector<Vertex> &vertices)
{
    GL_CALL(glGenBuffers(1, &m_RendererID));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                         vertices.data(), GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer() { GL_CALL(glDeleteBuffers(1, &m_RendererID)); }

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_RendererID(other.m_RendererID)
{
    other.m_RendererID = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this != &other)
    {
        GL_CALL(glDeleteBuffers(1, &m_RendererID));
        m_RendererID = other.m_RendererID;
        other.m_RendererID = 0;
    }
    return *this;
}

void VertexBuffer::Bind() const
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
void VertexBuffer::Unbind() const { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
