#include "renderer.hpp"

#include "camera.hpp"
#include "mesh.hpp"
#include "objs/IndexBuffer.hpp"
#include "objs/VertexArray.hpp"
#include "shaderprogram.hpp"
#include "utils.h"
#include <glad/glad.h>
#include <string>

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo,
                    const ShaderProgram& shaderProgram) const
{
    shaderProgram.Use();
    vao.Bind();
    ibo.Bind();
    GL_CALL(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, NULL));
}

void Renderer::DrawMesh(const Mesh& mesh, const ShaderProgram& shaderProgram,
                        const Camera& camera) const
{
    camera.UploadMatrix(shaderProgram, "u_ViewProj");

    shaderProgram.Use();
    mesh.GetVAO().Bind();
    mesh.GetIBO().Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (int i = 0; i < mesh.GetTextures().size(); ++i)
    {
        std::string num;
        std::string type = mesh.GetTextures()[i]->GetType();

        if (type == "diffuse")
        {
            num = std::to_string(numDiffuse++);
        }

        else if (type == "specular")
        {
            num = std::to_string(numSpecular++);
        }

        mesh.GetTextures()[i]->TexUnit(shaderProgram, (type + num).c_str(), i);
        mesh.GetTextures()[i]->Bind(i);
    }

    GL_CALL(glDrawElements(GL_TRIANGLES, mesh.GetIBO().GetCount(), GL_UNSIGNED_INT, NULL));
}

void Renderer::Clear(int r, int g, int b, int a, unsigned int mask) const
{
    GL_CALL(glClearColor(NORMALIZERGBA(r, g, b, a)));
    GL_CALL(glClear(mask));
}
