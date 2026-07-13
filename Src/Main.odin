package MyGameEngine

import "core:fmt"
import "core:math/linalg/glsl"
import "base:runtime"

import "vendor:glfw"
import gl "vendor:OpenGL"

main :: proc()
{
	app_init()

	mt1 := modeltype_add("Models/TutorialCube.obj")

	shader := shader_add(
		"Shaders/Default.vert","",
		"Shaders/Default.frag"
	)

	tex1 := texture_add("Textures/container.jpg")
	texture_bind(tex1, 0)
	shader_uniform(shader, "tex1", i32(0))

	m_mat : matrix[4,4]f32 = 1
	m_mat *= glsl.mat4Translate({0, 0.1, 0})
	//m_mat *= glsl.mat4Scale({0.5, 2.0, 1})
	m_mat *= glsl.mat4Rotate({1,0,0}, glsl.radians_f32(30))
	//m_mat *= glsl.mat4Rotate({0,1,0}, glsl.radians_f32(20))
	//m_mat *= glsl.mat4Rotate({0,0,1}, glsl.radians_f32(10))
	shader_uniform(shader, "m_mat", m_mat)

	default_camera = camera_make({-3,0,1}, {0,-30,0})
	shader_uniform(shader, "v_mat", default_camera.view_mat)

	default_proj_mat = set_proj_mat(90, 0.1, 100)
	shader_uniform(shader, "p_mat", default_proj_mat)

	move_speed : f32 = 0.1

	for !glfw.WindowShouldClose(window_handle)
	{
		glfw.PollEvents()

		default_camera.pos += move_speed * (
			f32(i32(key_is_down[.W]) - i32(key_is_down[.S])) * default_camera.dir_f +
			f32(i32(key_is_down[.A]) - i32(key_is_down[.D])) * default_camera.dir_l +
			f32(i32(key_is_down[.Q]) - i32(key_is_down[.E])) * default_camera.dir_u
		)
		camera_recalculate_view_mat(&default_camera)
		shader_uniform(shader, "v_mat", default_camera.view_mat)

		gl.ClearColor(0.5, 0.5, 0.5, 1.0)
		gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)

		//mesh_render(&mesh1)
		for &mesh in mt1.meshes {
			mesh_render(&mesh)
		}

		glfw.SwapBuffers(window_handle)
	}

	gl.DeleteProgram(shader)

	app_shutdown()
}
