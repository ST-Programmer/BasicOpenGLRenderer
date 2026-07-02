#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	void Bind() const;
	void Unbind() const;
	void AddBuffer(VertexBuffer &vbo, VertexBufferLayout &vblayout) const;

private:
	unsigned int m_RendererID;
};
