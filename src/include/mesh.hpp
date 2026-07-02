#pragma once

#include <vector>
#include <memory>
#include "objs/Vertex.hpp"
#include "objs/VertexArray.hpp"
#include "objs/VertexBuffer.hpp"
#include "objs/IndexBuffer.hpp"
#include "texture.hpp"

class Mesh
{
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
         std::vector<std::shared_ptr<Texture>>&& textures);

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept = default;
    Mesh& operator=(Mesh&& other) noexcept = default;

    inline const VertexArray& GetVAO() const
    {
        return m_VAO;
    }
    inline const IndexBuffer& GetIBO() const
    {
        return m_IBO;
    }
    inline const std::vector<std::shared_ptr<Texture>>& GetTextures() const
    {
        return m_Textures;
    }

private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<std::shared_ptr<Texture>> m_Textures;

    VertexArray m_VAO;
    VertexBuffer m_VBO;
    IndexBuffer m_IBO;
};
