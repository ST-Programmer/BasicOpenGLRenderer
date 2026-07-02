#include "mesh.hpp"
#include <memory>
#include "objs/VertexBufferLayout.hpp"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
           std::vector<std::shared_ptr<Texture>>&& textures)
    : m_Vertices(vertices), m_Indices(indices), m_Textures(std::move(textures)), m_VBO(vertices),
      m_IBO(indices)
{
    m_VAO.Bind();

    VertexBufferLayout layout;
    layout.Push<float>(3); // pos
    layout.Push<float>(3); // normal
    layout.Push<float>(3); // color
    layout.Push<float>(2); // uv
    m_VAO.AddBuffer(m_VBO, layout);
}
