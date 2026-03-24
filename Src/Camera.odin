package MyGameEngine

import "core:fmt"
import "core:math/linalg/glsl"

Camera :: struct
{
    using obj: Object,
    view_mat: matrix[4,4]f32,
    dir_f: [3]f32,
    dir_l: [3]f32,
    dir_u: [3]f32,
    roll_lock: bool //TEMP: Math only works if true
}

//TEMP: global camera variable
default_camera : Camera

//TODO: centralized lists to store Cameras & Objects etc?
//camera_list : [dynamic]Camera
_ :: 0

camera_make :: proc(pos: [3]f32, dir: [3]f32, roll_lock: bool = true) -> (cam: Camera)
{
    cam = Camera {
        pos = pos,
        dir = dir,
        roll_lock = roll_lock,
    }
    camera_update(&cam, 0,0,0)
    return
}

//TEMP
look_sensitivity : f32 = 0.125

camera_update :: proc(cam: ^Camera, d_pitch, d_yaw: f32, d_roll: f32 = 0)
{
    cam.dir.y += d_pitch * look_sensitivity
    cam.dir.z += d_yaw * look_sensitivity
    if !cam.roll_lock do cam.dir.x += d_roll

    if cam.dir.y < -89 {
        cam.dir.y = -89
    } else if cam.dir.y > 89 {
        cam.dir.y = 89
    }
    if cam.dir.z < 0 {
        cam.dir.z += 360
    } else if cam.dir.z >= 360 {
        cam.dir.z -= 360
    }

    if !cam.roll_lock {
        //TODO
    }

    //X forward, Y left, Z up
    cam.dir_f = glsl.normalize_vec3({
        glsl.cos(glsl.radians(cam.dir.z)) * glsl.cos(glsl.radians(cam.dir.y)),
        glsl.sin(glsl.radians(cam.dir.z)) * glsl.cos(glsl.radians(cam.dir.y)),
        glsl.sin(glsl.radians(cam.dir.y))
    })
    cam.dir_l = glsl.normalize(glsl.cross([3]f32{0,0,1}, cam.dir_f))
    cam.dir_u = glsl.normalize(glsl.cross(cam.dir_f, cam.dir_l))

    camera_recalculate_view_mat(cam)
}

camera_recalculate_view_mat :: proc(cam: ^Camera)
{
    //fmt.println(cam.pos, cam.forward)
    cam.view_mat = glsl.mat4LookAt(cam.pos, cam.pos + cam.dir_f, {0,0,1})
}

default_proj_mat : matrix[4,4]f32

set_proj_mat :: proc(fov_degrees: f32 = 90, near_z: f32 = 0.1, far_z: f32 = 100) -> matrix[4,4]f32
{
    return glsl.mat4Perspective(
        glsl.radians(fov_degrees),
        f32(window_width)/f32(window_height),
        near_z, far_z
    )
}
