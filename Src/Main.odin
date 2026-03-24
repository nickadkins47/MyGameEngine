package MyGameEngine

import "core:fmt"
import "core:math/linalg/glsl"
import "base:runtime"

import "vendor:glfw"
import gl "vendor:OpenGL"

main :: proc()
{
	app_init()

	VAO: u32
	gl.GenVertexArrays(1, &VAO)
	gl.BindVertexArray(VAO)

	VBO: u32
	gl.GenBuffers(1, &VBO)
	gl.BindBuffer(gl.ARRAY_BUFFER, VBO)

	tri_points := [?][8]f32 { //TEMP 3d pyramid thing using Triangle_Fan
		{   0,    0,  0.5, 0.0, 0.0, 0.0, 0.5, 1.0},

		{-0.5, -0.5, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0},
		{ 0.5, -0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0},
		{ 0.5,  0.5, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0},
		{-0.5,  0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0},
		{-0.5, -0.5, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0},
	}
	gl.BufferData(
		gl.ARRAY_BUFFER, size_of(tri_points), raw_data(&tri_points), gl.STATIC_DRAW
	)

	gl.VertexAttribPointer(0, 3, gl.FLOAT, false, 8*size_of(f32), 0*size_of(f32))
	gl.EnableVertexAttribArray(0)
	gl.VertexAttribPointer(1, 3, gl.FLOAT, false, 8*size_of(f32), 3*size_of(f32))
	gl.EnableVertexAttribArray(1)
	gl.VertexAttribPointer(2, 2, gl.FLOAT, false, 8*size_of(f32), 6*size_of(f32))
	gl.EnableVertexAttribArray(2)

	shader := shader_add(
		"Shaders/Default.vert","",
		"Shaders/Default.frag"
	)

	tex1 := texture_add("Textures/container.jpg")

	gl.ActiveTexture(gl.TEXTURE0)
	gl.BindTexture(gl.TEXTURE_2D, tex1)
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

		movements : [6]i32 = {
			cast(i32)key_is_down[.W], cast(i32)key_is_down[.S], // +/- Forward
			cast(i32)key_is_down[.A], cast(i32)key_is_down[.D], // +/- Left
			cast(i32)key_is_down[.Q], cast(i32)key_is_down[.E], // +/- Up
		}
		default_camera.pos += move_speed * (
			f32(movements[0] - movements[1]) * default_camera.dir_f +
			f32(movements[2] - movements[3]) * default_camera.dir_l +
			f32(movements[4] - movements[5]) * default_camera.dir_u
		)
		camera_recalculate_view_mat(&default_camera)
		shader_uniform(shader, "v_mat", default_camera.view_mat)

		gl.ClearColor(0.5, 0.5, 0.5, 1.0)
		gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)

		gl.DrawArrays(gl.TRIANGLE_FAN, 0, len(tri_points))

		glfw.SwapBuffers(window_handle)
	}

	gl.DeleteProgram(shader)
	gl.DeleteBuffers(1, &VBO)
	gl.DeleteBuffers(1, &VAO)

	app_shutdown()
}
