#include "VertexArray.hpp"

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "utils.h"
#include <glad/glad.h>

VertexArray::VertexArray() { GL_CALL(glGenVertexArrays(1, &m_RendererID)); }

VertexArray::~VertexArray() { GL_CALL(glDeleteVertexArrays(1, &m_RendererID)); }

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_RendererID(other.m_RendererID)
{
    other.m_RendererID = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other)
    {
        GL_CALL(glDeleteVertexArrays(1, &m_RendererID));
        m_RendererID = other.m_RendererID;
        other.m_RendererID = 0;
    }
    return *this;
}

void VertexArray::Bind() const { GL_CALL(glBindVertexArray(m_RendererID)); }

void VertexArray::Unbind() const { GL_CALL(glBindVertexArray(0)); }

void VertexArray::AddBuffer(VertexBuffer &vbo,
                            VertexBufferLayout &vblayout) const
{
    Bind();
    vbo.Bind();
    const std::vector<VertexBufferElement> &elements = vblayout.getElements();

    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        const VertexBufferElement &element = elements[i];
        GL_CALL(glEnableVertexAttribArray(i));
        GL_CALL(glVertexAttribPointer(i, element.count, element.type,
                                      element.normalized, vblayout.getStride(),
                                      (const void *)(uintptr_t)offset));
        offset +=
            element.count * VertexBufferElement::getTypeSize(element.type);
    }
}
