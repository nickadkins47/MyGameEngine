/**
 *   @file: Model.hh
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#pragma once

//#include <assimp/material.h>

#include "Core.hh"
#include "Mesh.hh"
#include "Shader.hh"

class aiNode;
class aiMesh;
class aiScene;
class aiMaterial;
enum aiTextureType;

class Model
{
    public:

    Model(string cref path);
    
    deleteOtherOps(Model)

    bool winding_cw = false; //render Clockwise (CW) instead of Counterclockwise (CCW)

    void render(Shader cref shader) const;

    //protected:

    vector<Mesh> meshes;
    
    //possible TODO: reorganize things once i understand scenes a little better

    void process_node(aiNode ptr node, aiScene cptr scene, string cref path);
    void process_mesh(aiMesh ptr mesh, aiScene cptr scene, string cref path);
    void process_material(Mesh ref mesh, aiMaterial cptr mat, aiTextureType type, string cref path);

};
