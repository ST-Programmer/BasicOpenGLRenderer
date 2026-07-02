#pragma once

#include "objs/VertexArray.hpp"
#include "objs/IndexBuffer.hpp"
#include "shaderprogram.hpp"
#include "mesh.hpp"
#include "camera.hpp"

class Renderer
{
public:
    void Draw(const VertexArray& vao, const IndexBuffer& ibo,
              const ShaderProgram& shaderProgram) const;
    void DrawMesh(const Mesh& mesh, const ShaderProgram& shaderProgram, const Camera& camera) const;
    void Clear(int r, int g, int b, int a, unsigned int mask) const;
};
