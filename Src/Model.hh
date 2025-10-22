/**
 *   @file: Model.hh
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Manager.hh"
#include "Mesh.hh"

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;
enum aiTextureType;
class Obj;
class Shader;

class Model : public Manager<Model>
{
    public:

    static bool add(string cref model_path, bool winding_cw = false, bool flip_uvs = true, bool instanced = false);

    static bool add(string cref model_name, vector<Mesh> cref meshes);

    delete_other_ops(Model)

    bool visible = true;
    bool winding_cw = false; //render Clockwise (CW) instead of Counterclockwise (CCW)
    bool instanced = false;

    vector<Mesh> meshes;
    vector<Obj ptr> parent_objs;

    Shader ptr shader = nullptr; //if this is nullptr, then it will use global default shader

    //possible TODO: reorganize things once i understand aiScenes a little better

    void import_node(aiNode ptr node, aiScene cptr scene, string cref model_dir, bool instanced);
    void import_mesh(aiMesh ptr mesh, aiScene cptr scene, string cref model_dir, bool instanced);

    static void import_material(Mesh ref mesh, aiMaterial cptr mat, string cref model_dir);

    Model(); //do not manually call. always use _::add() instead
};
