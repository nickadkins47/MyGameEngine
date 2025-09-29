/**
 *   @file: Model.hh
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Mesh.hh"

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;
enum aiTextureType;
class Shader;

class Model
{
    public:

    static optional<Model ptr> add(string cref model_path, bool winding_cw = false, bool flip_uvs = true);

    static optional<Model ptr> get(string cref model_name);

    static bool exists(string cref model_name);
    
    delete_other_ops(Model)

    bool winding_cw = false; //render Clockwise (CW) instead of Counterclockwise (CCW)

    void render(Shader cptr shader) const;

    //protected:

    Model();

    unordered_map<string, Model> inline static model_map;

    vector<Mesh> meshes;
    
    //possible TODO: reorganize things once i understand aiScenes a little better

    void import_node(aiNode ptr node, aiScene cptr scene, string cref model_dir);
    void import_mesh(aiMesh ptr mesh, aiScene cptr scene, string cref model_dir);
    
    static void import_material(Mesh ref mesh, aiMaterial cptr mat, string cref model_dir);

};
