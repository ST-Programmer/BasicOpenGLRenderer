#pragma once

#include <assimp/scene.h>
#include <string>
#include <vector>
#include <memory>
#include "unordered_map"
#include "renderer.hpp"
#include "shaderprogram.hpp"
#include "camera.hpp"
#include "mesh.hpp"

class Model
{
public:
    Model(const std::string& filePath);
    void Draw(const Renderer& renderer, const ShaderProgram& shaderProgram,
              const Camera& camera) const;

private:
    std::vector<Mesh> m_Meshes;
    std::string m_FilePath;
    std::string m_Directory;
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_LoadedTextures;

    void LoadModel();
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture>>
    LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);
};