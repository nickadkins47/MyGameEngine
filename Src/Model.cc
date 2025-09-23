/**
 *   @file: Model.cc
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Engine.hh"
#include "Model.hh"

Model::Model() {}

optional<Model ptr> Model::add(path cref model_p, bool flip_uvs)
{
    Log::info("Adding model \"{}\" via Assimp...", model_p.string());
    Assimp::Importer importer;

    aiScene cptr scene = importer.ReadFile(model_p.string(), 
        aiProcess_Triangulate | (flip_uvs ? aiProcess_FlipUVs : 0)
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        Log::warn("Adding model \"{}\" via Assimp: Failed ({})", 
            model_p.string(), importer.GetErrorString()
        );
        return nullopt;
    }

    Model ptr model = &engine->model_map[model_p.string()];

    model->import_node(scene->mRootNode, scene, model_p.parent_path());

    Log::info("Adding model \"{}\" via Assimp: Success", model_p.string());
    return model;
}

optional<Model ptr> Model::get(string cref model_name)
{
    Log::info("Getting model \"{}\"...", model_name);
    auto iter = engine->model_map.find(model_name);
    if (iter == engine->model_map.end())
    {
        Log::warn("Getting model \"{}\": Failed", model_name);
        return nullopt;
    }
    else
    {
        Log::info("Getting model \"{}\": Success", model_name);
        return &iter->second;
    }
}

bool Model::exists(string cref model_name)
{
    return engine->model_map.contains(model_name);
}

void Model::render(Shader cptr shader) const
{
    glFrontFace(winding_cw ? GL_CW : GL_CCW);

    for (Mesh cref mesh : meshes)
        mesh.render(shader);
}

void Model::import_node(aiNode ptr node, aiScene cptr scene, path cref file_p)
{
    for (uint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh ptr mesh = scene->mMeshes[node->mMeshes[i]];
        import_mesh(mesh, scene, file_p);
    }

    for (uint i = 0; i < node->mNumChildren; i++)
    {
        import_node(node->mChildren[i], scene, file_p);
    }
}

void Model::import_mesh(aiMesh ptr a_mesh, aiScene cptr scene, path cref file_p)
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
        import_material(mesh, material, aiTextureType_DIFFUSE, file_p);
        import_material(mesh, material, aiTextureType_SPECULAR, file_p);
    }

    mesh.gen_gl_data();
}

void Model::import_material(Mesh ref mesh, aiMaterial cptr mat, aiTextureType type, path cref file_p)
{
    for (uint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString texture_fn; //texture's file name
        mat->GetTexture(type, i, &texture_fn);
        path const texture_p (file_p.string() + '/' + texture_fn.C_Str());

        //Get Texture, or add it if it doenst exist already
        Texture ptr texture = (Texture::exists(texture_p.string()))
            ? Texture::get(texture_p.string()).value()
            : Texture::add(texture_p).value()
        ;

        texture->type = type;
        mesh.textures.push_back(texture);
    }
}