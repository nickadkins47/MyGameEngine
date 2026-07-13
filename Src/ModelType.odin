package MyGameEngine

import "core:fmt"

import gl "vendor:OpenGL"

import "Depends/Assimp"

ModelType :: struct
{
    meshes: [dynamic]Mesh
}

modeltype_add :: proc(path: cstring) -> (modeltype: ModelType)
{
	scene := Assimp.import_file(path, u32(Assimp.aiPostProcessSteps.Triangulate))

	if scene == nil \
		|| bool(scene.mFlags & u32(Assimp.aiSceneFlags.INCOMPLETE)) \
		|| scene.mRootNode == nil
	{
		fmt.panicf("Model \'%v\' cannot be found", path)
	}

	modeltype_import_node(&modeltype, scene, scene.mRootNode)

	return
}

modeltype_remove :: proc(mt: ^ModelType)
{
	delete(mt.meshes)
}

@(private="file")
modeltype_import_node :: proc(mt: ^ModelType, scene: ^Assimp.aiScene, node: ^Assimp.aiNode)
{
	for i in 0..<node.mNumMeshes {
		mesh := scene.mMeshes[node.mMeshes[i]]
		modeltype_import_mesh(mt, scene, mesh)
	}

	for i in 0..<node.mNumChildren {
		modeltype_import_node(mt, scene, node.mChildren[i])
	}
}

@(private="file")
modeltype_import_mesh :: proc(mt: ^ModelType, scene: ^Assimp.aiScene, a_mesh: ^Assimp.aiMesh)
{
	mesh := mesh_add()

	for i in 0..<a_mesh.mNumVertices {
		append(&mesh.vertices, Vertex{
			x  = a_mesh.mVertices[i].x,
			y  = a_mesh.mVertices[i].y,
			z  = a_mesh.mVertices[i].z,
			nx = a_mesh.mNormals[i].x,
			ny = a_mesh.mNormals[i].y,
			nz = a_mesh.mNormals[i].z,
			tx = (a_mesh.mTextureCoords[0] != nil) \
				? a_mesh.mTextureCoords[0][i].x : 0,
			ty = (a_mesh.mTextureCoords[0] != nil) \
				? a_mesh.mTextureCoords[0][i].y : 0,
		})
	}

	for i in 0..<a_mesh.mNumFaces {
		face := a_mesh.mFaces[i]
		for j in 0..<face.mNumIndices {
			append(&mesh.indices, face.mIndices[j])
		}
	}

	mesh_finalize_gl_data(&mesh, gl.TRIANGLES, is_indexed=true)
	append(&mt.meshes, mesh)
}
