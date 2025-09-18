/**
 *   @file: Model.cc
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Engine.hh"
#include "Model.hh"

Model::Model(string cref path)
{
    Assimp::Importer importer;

    bool flip_uvs = true;

    if (std::filesystem::path(path).extension() == ".dae")
        flip_uvs = false;

    aiScene cptr scene = importer.ReadFile(get_file_path(path), 
        aiProcess_Triangulate | (flip_uvs ? aiProcess_FlipUVs : 0)
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        print("ERROR::ASSIMP::{}\n", importer.GetErrorString());
        return;
    }

    process_node(scene->mRootNode, scene, std::filesystem::path(path).parent_path().string());
}

void Model::render(Shader cref shader) const
{
    glFrontFace(winding_cw ? GL_CW : GL_CCW);

    for (Mesh cref mesh : meshes)
        mesh.render(shader);
}

void Model::process_node(aiNode ptr node, aiScene cptr scene, string cref path)
{
    for (uint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh ptr mesh = scene->mMeshes[node->mMeshes[i]];
        process_mesh(mesh, scene, path);
    }

    for (uint i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene, path);
    }
}

void Model::process_mesh(aiMesh ptr a_mesh, aiScene cptr scene, string cref path)
{
    Mesh ref mesh = meshes.emplace_back();

    for (uint i = 0; i < a_mesh->mNumVertices; i++)
    {
        Vertex ref vertex = mesh.vertices.emplace_back();
        
        vertex.x = a_mesh->mVertices[i].x;
        vertex.y = a_mesh->mVertices[i].y;
        vertex.z = a_mesh->mVertices[i].z;
        vertex.nx = a_mesh->mNormals[i].x;
        vertex.ny = a_mesh->mNormals[i].y;
        vertex.nz = a_mesh->mNormals[i].z;

        if (a_mesh->mTextureCoords[0])
        {
            vertex.tx = a_mesh->mTextureCoords[0][i].x;
            vertex.ty = a_mesh->mTextureCoords[0][i].y;
        }
    }

    for (uint i = 0; i < a_mesh->mNumFaces; i++)
    {
        aiFace ref face = a_mesh->mFaces[i];
        for (uint j = 0; j < face.mNumIndices; j++)
            mesh.indices.push_back(face.mIndices[j]);
    }

    if (a_mesh->mMaterialIndex >= 0)
    {
        aiMaterial ptr material = scene->mMaterials[a_mesh->mMaterialIndex];
        process_material(mesh, material, aiTextureType_DIFFUSE, path);
        process_material(mesh, material, aiTextureType_SPECULAR, path);
    }

    mesh.gen_gl_data();
}

void Model::process_material(Mesh ref mesh, aiMaterial cptr mat, aiTextureType type, string cref path)
{
    for (uint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture ref texture = engine->get_texture(path + '/' + string(str.C_Str()));
        texture.type = type;
        mesh.textures.push_back(&texture);
    }
}