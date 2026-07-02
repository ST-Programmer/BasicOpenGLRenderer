#pragma once

#include <vector>
#include <cassert>
#include <glad/glad.h>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getTypeSize(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return sizeof(GLfloat);
			break;
		case GL_UNSIGNED_INT:
			return sizeof(GLuint);
			break;
		case GL_UNSIGNED_BYTE:
			return sizeof(GLbyte);
			break;
		}

		assert(false);
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() : m_Stride(0) {}

	template <typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template <>
	void Push<float>(unsigned int count)
	{
		m_Elements.emplace_back(VertexBufferElement{GL_FLOAT, count, GL_FALSE});
		m_Stride += count * VertexBufferElement::getTypeSize(GL_FLOAT);
	}

	template <>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.emplace_back(VertexBufferElement{GL_UNSIGNED_INT, count, GL_FALSE});
		m_Stride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_INT);
	}

	template <>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.emplace_back(VertexBufferElement{GL_UNSIGNED_BYTE, count, GL_TRUE});
		m_Stride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> &getElements() const { return m_Elements; }
	inline unsigned int getStride() const { return m_Stride; }

private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
};
