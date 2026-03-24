package MyGameEngine

import "core:fmt"
import "base:runtime"

import "vendor:glfw"
import gl "vendor:OpenGL"

gl_major_ver :: 4
gl_minor_ver :: 6

window_name: cstring = "MyGameEngine"
window_width: i32 = 800
window_height: i32 = 600

window_handle: glfw.WindowHandle

window_init :: proc()
{
    if !glfw.Init() {
        fmt.panicf("Couldn't initialize GLFW\n")
    }

    glfw.WindowHint(glfw.CONTEXT_VERSION_MAJOR, gl_major_ver) //OpenGL 4.6 Core
	glfw.WindowHint(glfw.CONTEXT_VERSION_MINOR, gl_minor_ver)
	glfw.WindowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

    glfw.WindowHint(glfw.SCALE_FRAMEBUFFER, false)
	glfw.WindowHint(glfw.SAMPLES, 4)

    window_handle = glfw.CreateWindow(window_width, window_height, window_name, nil, nil)

    if window_handle == nil {
        fmt.panicf("Couldn't Initialize GLFW Window\n")
    }

    glfw.MakeContextCurrent(window_handle)
    glfw.SwapInterval(1)

    gl.load_up_to(gl_major_ver, gl_minor_ver, glfw.gl_set_proc_address)

    gl.Enable(gl.DEPTH_TEST)
    gl.Enable(gl.MULTISAMPLE)

    glfw.SetFramebufferSizeCallback(window_handle, framebuffer_size_callback)
    glfw.SetKeyCallback(window_handle, key_callback)
    glfw.SetMouseButtonCallback(window_handle, mouse_button_callback)
    glfw.SetCursorPosCallback(window_handle, mouse_pos_callback)
}

window_shutdown :: proc()
{
    glfw.DestroyWindow(window_handle)
    glfw.Terminate()
}