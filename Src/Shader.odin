package MyGameEngine

import "core:fmt"
import "core:os"
import "core:strings"
import "core:math/linalg/glsl"

import gl "vendor:OpenGL"

shader_add :: proc(vert_path, geom_path, frag_path: string) -> (shader_prog: u32)
{
    do_geom := (geom_path != "")

    vert_id := shader_add_part(vert_path, gl.VERTEX_SHADER)
    geom_id := shader_add_part(geom_path, gl.GEOMETRY_SHADER) if do_geom else 0
    frag_id := shader_add_part(frag_path, gl.FRAGMENT_SHADER)

    shader_prog = gl.CreateProgram()

    gl.AttachShader(shader_prog, vert_id)
    if do_geom do gl.AttachShader(shader_prog, geom_id)
    gl.AttachShader(shader_prog, frag_id)
    
    gl.LinkProgram(shader_prog)

    success : i32
    gl.GetProgramiv(shader_prog, gl.LINK_STATUS, &success)
	if success != 1
	{
        info_log : [512]u8
		gl.GetProgramInfoLog(shader_prog, 512, nil, raw_data(&info_log))
		fmt.panicf(string(info_log[:]))
	}

    gl.UseProgram(shader_prog)

    gl.DeleteShader(vert_id)
    if do_geom do gl.DeleteShader(geom_id)
	gl.DeleteShader(frag_id)

    return
}

shader_uniform :: proc
{
    shader_uniform_f,
    shader_uniform_i,
    shader_uniform_u,
    shader_uniform_fv,
    shader_uniform_iv,
    shader_uniform_uv,
    shader_uniform_mat,
    shader_uniform_q,
}

shader_uniform_f :: proc(shader: u32, name: cstring, data: f32)
{
    gl.Uniform1f(gl.GetUniformLocation(shader, name), data)
}

shader_uniform_i :: proc(shader: u32, name: cstring, data: i32)
{
    gl.Uniform1i(gl.GetUniformLocation(shader, name), data)
}

shader_uniform_u :: proc(shader: u32, name: cstring, data: u32)
{
    gl.Uniform1ui(gl.GetUniformLocation(shader, name), data)
}

shader_uniform_fv :: proc(shader: u32, name: cstring, data: [$L]f32, count: i32 = 1)
    where 1 <= L && L <= 4
{
    data := data
    loc := gl.GetUniformLocation(shader, name)
    switch L
    {
        case 1: gl.Uniform1fv(loc, count, raw_data(&data))
        case 2: gl.Uniform2fv(loc, count, raw_data(&data))
        case 3: gl.Uniform3fv(loc, count, raw_data(&data))
        case 4: gl.Uniform4fv(loc, count, raw_data(&data))
    }
}

shader_uniform_iv :: proc(shader: u32, name: cstring, data: [$L]i32, count: i32 = 1)
    where 1 <= L && L <= 4
{
    data := data
    loc := gl.GetUniformLocation(shader, name)
    switch L
    {
        case 1: gl.Uniform1iv(loc, count, raw_data(&data))
        case 2: gl.Uniform2iv(loc, count, raw_data(&data))
        case 3: gl.Uniform3iv(loc, count, raw_data(&data))
        case 4: gl.Uniform4iv(loc, count, raw_data(&data))
    }
}

shader_uniform_uv :: proc(shader: u32, name: cstring, data: [$L]u32, count: i32 = 1)
    where 1 <= L && L <= 4
{
    data := data
    loc := gl.GetUniformLocation(shader, name)
    switch L
    {
        case 1: gl.Uniform1uiv(loc, count, raw_data(&data))
        case 2: gl.Uniform2uiv(loc, count, raw_data(&data))
        case 3: gl.Uniform3uiv(loc, count, raw_data(&data))
        case 4: gl.Uniform4uiv(loc, count, raw_data(&data))
    }
}

shader_uniform_mat :: proc(shader: u32, name: cstring, data: matrix[$R,$C]f32,
    count: i32 = 1, transpose: bool = false)
    where (R == 2 || R == 3 || R == 4) && (C == 2 || C == 3 || C == 4)
{
    data := data
    loc := gl.GetUniformLocation(shader, name)
    //NOTE: converting OdinMat -> GlslMat req. using opposite coordinates
    //IE Odin's matrix[2,4] uses glsl's mat4x2
    switch R
    {
        case 2: switch C
        {
            case 2: gl.UniformMatrix2fv  (loc, count, transpose, raw_data(&data))
            case 3: gl.UniformMatrix3x2fv(loc, count, transpose, raw_data(&data))
            case 4: gl.UniformMatrix4x2fv(loc, count, transpose, raw_data(&data))
        }
        case 3: switch C
        {
            case 2: gl.UniformMatrix2x3fv(loc, count, transpose, raw_data(&data))
            case 3: gl.UniformMatrix3fv  (loc, count, transpose, raw_data(&data))
            case 4: gl.UniformMatrix4x3fv(loc, count, transpose, raw_data(&data))
        }
        case 4: switch C
        {
            case 2: gl.UniformMatrix2x4fv(loc, count, transpose, raw_data(&data))
            case 3: gl.UniformMatrix3x4fv(loc, count, transpose, raw_data(&data))
            case 4: gl.UniformMatrix4fv  (loc, count, transpose, raw_data(&data))
        }
    }
}

shader_uniform_q :: proc(shader: u32, name: cstring, data: quaternion128, count: i32 = 1)
{
    shader_uniform_fv(shader, name, transmute([4]f32)(data), count)
}

/* shader_sampler2d :: proc(shader: u32, ) */

@(private="file")
shader_add_part :: proc(path: string, type: u32) -> (part: u32)
{
    part = gl.CreateShader(type)
    part_bytes, err := os.read_entire_file(path, context.allocator)
    if err != nil do fmt.panicf("ERROR: %v\n", err)

    part_data := strings.clone_to_cstring(string(part_bytes))
    gl.ShaderSource(part, 1, &part_data, nil)
	gl.CompileShader(part)

    success : i32
    gl.GetShaderiv(part, gl.COMPILE_STATUS, &success)
	if success != 1
	{
        info_log : [512]u8
		gl.GetShaderInfoLog(part, 512, nil, raw_data(&info_log))
		fmt.panicf(string(info_log[:]))
	}

    return
}