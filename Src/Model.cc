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
#include <glm/gtc/type_ptr.hpp>

#include "Engine.hh"
#include "Model.hh"
#include "Shader.hh"
#include "Texture.hh"

Model::Model() {}

optional<Model ptr> Model::add(string cref model_path, bool winding_cw, bool flip_uvs, bool instanced)
{
    Log::info("Adding model \"{}\" via Assimp...", model_path);
    Assimp::Importer importer;

    aiScene cptr scene = importer.ReadFile(model_path, 
        aiProcess_Triangulate | (flip_uvs ? aiProcess_FlipUVs : 0)
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        Log::warn("Adding model \"{}\" via Assimp: Failed ({})", 
            model_path, importer.GetErrorString()
        );
        return nullopt;
    }

    Model ptr model = manager.get_new(model_path);
    model->winding_cw = winding_cw;
    model->instanced = instanced;

    string const model_dir = std::filesystem::path(model_path).parent_path().string();
    model->import_node(scene->mRootNode, scene, model_dir, instanced);

    Log::info("Adding model \"{}\" via Assimp: Success", model_path);
    return model;
}

optional<Model ptr> Model::add(string cref model_name, vector<Mesh> cref meshes)
{
    Log::info("Adding model \"{}\" directly...", model_name);

    Model ptr model = manager.get_new(model_name);
    model->meshes = meshes;
    for (auto ref mesh : model->meshes)
        mesh.parent = model;

    Log::info("Adding model \"{}\" directly: Success.", model_name);
    return model;
}

manager_funcs_cc(Model, model_name)

void Model::render() const
{
    Shader ptr shader = (this->shader == nullptr)
        ? engine->default_shader
        : this->shader
    ;
    shader->use();

    glFrontFace(winding_cw ? GL_CW : GL_CCW);

    if (instanced)
    {
        //model mat buffer (IVBO) should already be set
        for (auto ref mesh : meshes)
            mesh.render(shader);
    }
    else //not instanced -> render normally
    {
        for (auto obj : parent_objs)
        {
            shader->uniform_fm("m_mat", 4,4, glm::value_ptr(obj->model_mat));
            for (auto ref mesh : meshes)
                mesh.render(shader);
        }
    }
}

void Model::import_node(aiNode ptr node, aiScene cptr scene, string cref model_dir, bool instanced)
{
    for (uint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh ptr mesh = scene->mMeshes[node->mMeshes[i]];
        import_mesh(mesh, scene, model_dir, instanced);
    }

    for (uint i = 0; i < node->mNumChildren; i++)
    {
        import_node(node->mChildren[i], scene, model_dir, instanced);
    }
}

void Model::import_mesh(aiMesh ptr a_mesh, aiScene cptr scene, string cref model_dir, bool instanced)
{
    Mesh ref mesh = meshes.emplace_back(this);

    //TODO: mesh.vertices.reserve() for however many vertices there are
    for (uint i = 0; i < a_mesh->mNumVertices; i++)
    {
        mesh.vertices.emplace_back() = {
            .x = a_mesh->mVertices[i].x,
            .y = a_mesh->mVertices[i].y,
            .z = a_mesh->mVertices[i].z,
            .nx = a_mesh->mNormals[i].x,
            .ny = a_mesh->mNormals[i].y,
            .nz = a_mesh->mNormals[i].z,
            .tx = (a_mesh->mTextureCoords[0])
                ? a_mesh->mTextureCoords[0][i].x
                : 0,
            .ty = (a_mesh->mTextureCoords[0])
                ? a_mesh->mTextureCoords[0][i].y
                : 0,
        };
    }

    for (uint i = 0; i < a_mesh->mNumFaces; i++)
    {
        aiFace ref face = a_mesh->mFaces[i];
        for (uint j = 0; j < face.mNumIndices; j++)
            mesh.indices.push_back(face.mIndices[j]);
    }

    if (a_mesh->mMaterialIndex >= 0)
        import_material(mesh, scene->mMaterials[a_mesh->mMaterialIndex], model_dir);

    mesh.gen_gl_data(true, instanced);
}

void Model::import_material(Mesh ref mesh, aiMaterial cptr mat, string cref model_dir)
{
    auto static constexpr supported_tex_types = {
        aiTextureType_DIFFUSE,
        aiTextureType_SPECULAR,
        aiTextureType_AMBIENT
    };
    for (aiTextureType type : supported_tex_types)
    {
        for (uint i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString texture_fn; //texture's file name
            mat->GetTexture(type, i, &texture_fn);
            string const texture_path (model_dir + '/' + texture_fn.C_Str());

            //Get Texture, or add it if it doenst exist already
            Texture ptr texture = (Texture::exists(texture_path))
                ? Texture::get(texture_path).value()
                : Texture::add(texture_path, type).value()
            ;
            mesh.textures.push_back(texture);
        }
    }

    /* aiReturn got_shiny = aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &mesh.shininess);
    if (got_shiny == AI_SUCCESS)
    {
        //Scale down from ~ 0-1000 -> ~ 0-125 (Within OpenGL's limit of 128)
        mesh.shininess /= 8.0f;
    }
    else mesh.shininess = 32.0f; //Default */
}
