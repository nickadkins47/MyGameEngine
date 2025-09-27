/**
 *   @file: Model.hh
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Mesh.hh"
#include "Shader.hh"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

class Model
{
    public:

    static optional<Model ptr> add(path cref model_p, bool winding_cw = false, bool flip_uvs = true);

    static optional<Model ptr> get(string cref model_name);

    static bool exists(string cref model_name);
    
    deleteOtherOps(Model)

    bool winding_cw = false; //render Clockwise (CW) instead of Counterclockwise (CCW)

    void render(Shader cptr shader) const;

    //protected:

    Model();

    vector<Mesh> meshes;
    
    //possible TODO: reorganize things once i understand aiScenes a little better

    void import_node(aiNode ptr node, aiScene cptr scene, path cref file_p);
    void import_mesh(aiMesh ptr mesh, aiScene cptr scene, path cref file_p);
    
    static void import_material(Mesh ref mesh, aiMaterial cptr mat, path cref file_p);

};
