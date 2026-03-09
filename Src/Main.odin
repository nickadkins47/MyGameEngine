package main

import "vendor:glfw/bindings"
import "core:fmt"
import "core:os"
import "core:strings"
import "core:c"

import "vendor:glfw"
import gl "vendor:OpenGL"

main :: proc() {

    if !glfw.Init() {
        fmt.println("Couldn't initialize GLFW")
        return
    }

    glfw.WindowHint(glfw.CONTEXT_VERSION_MAJOR, 4)
    glfw.WindowHint(glfw.CONTEXT_VERSION_MINOR, 6)
    glfw.WindowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
    glfw.WindowHint(glfw.SAMPLES, 4)

    window := glfw.CreateWindow(800, 600, "Test", nil, nil)

    defer glfw.Terminate()
    defer glfw.DestroyWindow(window)

    if window == nil {
        fmt.println("Couldn't Initialize GLFW Window")
        return
    }

    glfw.MakeContextCurrent(window)
    glfw.SwapInterval(1)

    gl.load_up_to(4,6, glfw.gl_set_proc_address)

	glfw.SetFramebufferSizeCallback(window,
		proc "c" (window: bindings.WindowHandle, width: i32, height: i32) {
			gl.Viewport(0,0, width, height)
		}
	)

	{
		w,h := glfw.GetFramebufferSize(window)
		gl.Viewport(0,0,w,h)
	}
    
	VAO : u32
    gl.GenVertexArrays(1, &VAO)
    gl.BindVertexArray(VAO)
	defer gl.DeleteVertexArrays(1, &VAO)

	VBO : u32
    gl.GenBuffers(1, &VBO)
    gl.BindBuffer(gl.ARRAY_BUFFER, VBO)	
	defer gl.DeleteBuffers(1, &VBO)

    tri_points := [3][8]f32 {
        {-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
        { 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
        {   0,  0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}
    }
    gl.BufferData(gl.ARRAY_BUFFER, size_of(tri_points), raw_data(&tri_points), gl.STATIC_DRAW)

    gl.VertexAttribPointer(0, 3, gl.FLOAT, false, 8*size_of(f32), 0*size_of(f32))
    gl.EnableVertexAttribArray(0)
    gl.VertexAttribPointer(1, 3, gl.FLOAT, false, 8*size_of(f32), 3*size_of(f32))
    gl.EnableVertexAttribArray(1)
    gl.VertexAttribPointer(2, 2, gl.FLOAT, false, 8*size_of(f32), 6*size_of(f32))
    gl.EnableVertexAttribArray(2)

    vert_name := "Shaders/Default.vert"
    vert := gl.CreateShader(gl.VERTEX_SHADER)
    vert_bytes, v_err := os.read_entire_file(vert_name, context.allocator)
    vert_data :=  strings.clone_to_cstring(string(vert_bytes))
    if (v_err != nil) {
        fmt.eprintln("ERROR:", v_err)
    }
    gl.ShaderSource(vert, 1, &vert_data, nil)
    gl.CompileShader(vert)

    success : i32
    info_log : [512]c.char

    gl.GetShaderiv(vert, gl.COMPILE_STATUS, &success)
    if success != 1 {
        gl.GetShaderInfoLog(vert, 512, nil, raw_data(&info_log))
        fmt.eprintln(info_log)
        return
    }

    frag_name := "Shaders/Default.frag"
    frag := gl.CreateShader(gl.FRAGMENT_SHADER)
    frag_bytes, f_err := os.read_entire_file(frag_name, context.allocator)
    frag_data :=  strings.clone_to_cstring(string(frag_bytes))
    if (f_err != nil) {
        fmt.eprintln("ERROR:", f_err)
    }
    gl.ShaderSource(frag, 1, &frag_data, nil)
    gl.CompileShader(frag)

    gl.GetShaderiv(frag, gl.COMPILE_STATUS, &success)
    if success != 1 {
        gl.GetShaderInfoLog(frag, 512, nil, raw_data(&info_log))
        fmt.eprintln(info_log)
        return
    }

    shader_prog := gl.CreateProgram()
    gl.AttachShader(shader_prog, vert)
    gl.AttachShader(shader_prog, frag)
    gl.LinkProgram(shader_prog)

    gl.GetProgramiv(shader_prog, gl.LINK_STATUS, &success)
    if success != 1 {
        gl.GetProgramInfoLog(shader_prog, 512, nil, raw_data(&info_log))
        fmt.eprintln(info_log)
        return
    }

    gl.UseProgram(shader_prog)
    gl.DeleteShader(vert)
    gl.DeleteShader(frag)
	defer gl.DeleteProgram(shader_prog)

    for !glfw.WindowShouldClose(window) {
		glfw.PollEvents()

 		gl.ClearColor(0.5, 0.5, 0.5, 1.0)
		gl.Clear(gl.COLOR_BUFFER_BIT)

		gl.DrawArrays(gl.TRIANGLES, 0, 3)

		glfw.SwapBuffers(window)
    }
}