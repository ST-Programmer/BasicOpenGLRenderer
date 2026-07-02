#pragma once

#include <vector>
#include "objs/Vertex.hpp"

class VertexBuffer
{
public:
	VertexBuffer(const void *data, unsigned int size);
	VertexBuffer(std::vector<Vertex> &vertices);
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;
	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};
