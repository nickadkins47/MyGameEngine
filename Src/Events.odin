package MyGameEngine

import "core:fmt"
import "base:runtime"

import "vendor:glfw"
import gl "vendor:OpenGL"

framebuffer_size_callback :: proc "c" (window: glfw.WindowHandle, width, height: i32)
{
    window_width = width
    window_height = height
    gl.Viewport(0,0, width, height)
}

key_callback :: proc "c" (window: glfw.WindowHandle, key, scancode, action, mods: i32)
{
    context = runtime.default_context()
    fmt.println("EVENT: KeyAction", KeyList(key), scancode, action, mods)

    keyval := KeyList(key)
    if action == glfw.PRESS {
        key_callback := key_callback_map[keyval]
        if key_callback != nil {
            key_callback()
        }
        key_is_down[keyval] = true
    } else if action == glfw.RELEASE {
        key_is_down[keyval] = false
    }
}

//TEMP: stores map of key IDs to callbacks for when key is pressed
//
//TODO: proper setup for key callbacks later on
key_callback_map: map[KeyList](#type proc())

//TEMP: stores bool for if key is down
key_is_down: map[KeyList]bool

mouse_button_callback :: proc "c" (window: glfw.WindowHandle, button, action, mods: i32)
{
    context = runtime.default_context()
    fmt.println("EVENT: MouseClick", button, action, mods)

    //TEMP
    if button == glfw.MOUSE_BUTTON_LEFT {
        if action == glfw.PRESS {
            camera_lock = true
            first_mouse = true
            glfw.SetInputMode(window, glfw.CURSOR, glfw.CURSOR_DISABLED)
        } else if action == glfw.RELEASE {
            camera_lock = false
            glfw.SetInputMode(window, glfw.CURSOR, glfw.CURSOR_NORMAL)
        }
    }
}

//TEMP
camera_lock : bool = false
first_mouse : bool = false

mouse_pos_callback :: proc "c" (window: glfw.WindowHandle, xpos, ypos: f64)
{
    context = runtime.default_context()
    //fmt.println("EVENT: MouseMove", xpos, ypos)

    //TEMP: directly control default_camera
    xpos := f32(xpos)
    ypos := f32(ypos)

    @static old_xpos : f32
    @static old_ypos : f32

    if first_mouse {
        old_xpos = xpos
        old_ypos = ypos
        first_mouse = false
    }

    if camera_lock {
        camera_update(&default_camera, -(ypos - old_ypos), -(xpos - old_xpos))
    }

    old_xpos = xpos
    old_ypos = ypos
}