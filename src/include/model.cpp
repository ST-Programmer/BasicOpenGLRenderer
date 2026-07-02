#include "model.hpp"

#include "utils.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "renderer.hpp"
#include "shaderprogram.hpp"
#include "texture.hpp"
#include "objs/Vertex.hpp"
#include "mesh.hpp"

Model::Model(const std::string& filePath) : m_FilePath(filePath)
{
    LoadModel();
}

void Model::Draw(const Renderer& renderer, const ShaderProgram& shaderProgram,
                 const Camera& camera) const
{
    for (const auto& mesh : m_Meshes)
    {
        renderer.DrawMesh(mesh, shaderProgram, camera);
    }
}

void Model::LoadModel()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        ERR(std::string("Error Loading Assimp: ") + importer.GetErrorString());
        return;
    }

    m_Directory = m_FilePath.substr(0, m_FilePath.find_last_of('/') + 1);

    ProcessNode(scene->mRootNode, scene);
}
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}
Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;
    textures.reserve(2);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        glm::vec3 vertexPos;
        vertexPos.x = mesh->mVertices[i].x;
        vertexPos.y = mesh->mVertices[i].y;
        vertexPos.z = mesh->mVertices[i].z;

        glm::vec3 vertexNorm;
        vertexNorm.x = mesh->mNormals[i].x;
        vertexNorm.y = mesh->mNormals[i].y;
        vertexNorm.z = mesh->mNormals[i].z;

        // For Now "No" Color
        glm::vec3 vertexColor(1.0f, 1.0f, 1.0f);
        glm::vec2 vertexTexCoords(0.0f, 0.0f);

        if (mesh->HasTextureCoords(0))
        {
            vertexTexCoords.x = mesh->mTextureCoords[0][i].x;
            vertexTexCoords.y = mesh->mTextureCoords[0][i].y;
        }

        vertex.pos = vertexPos;
        vertex.normal = vertexNorm;
        vertex.color = vertexColor;
        vertex.uv = vertexTexCoords;

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<std::shared_ptr<Texture>> diffuseMaps =
        LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<std::shared_ptr<Texture>> specularMaps =
        LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return Mesh(vertices, indices, std::move(textures));
}

std::vector<std::shared_ptr<Texture>>
Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName)
{
    std::vector<std::shared_ptr<Texture>> textures;
    textures.reserve(material->GetTextureCount(type));

    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        std::string fullPath = m_Directory + str.C_Str();
        auto it = m_LoadedTextures.find(fullPath);

        if (it != m_LoadedTextures.end())
        {
            textures.push_back(it->second);
        }

        else
        {
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(fullPath, typeName);
            m_LoadedTextures[fullPath] = texture;
            textures.push_back(texture);
        }
    }
    return textures;
}