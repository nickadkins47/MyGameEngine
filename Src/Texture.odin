package MyGameEngine

import "core:fmt"
import "core:strings"

import gl "vendor:OpenGL"
import stbi "vendor:stb/image"

texture_add :: proc(tex_path: string) -> (texture: u32)
{
    stbi.flip_vertically_on_write(true)

    width, height, src_channels: i32
    
    data := stbi.load(strings.clone_to_cstring(tex_path), &width, &height, &src_channels, 4)
    if data == nil {
        fmt.panicf("Cannot load texture %v", tex_path)
    }

    gl.GenTextures(1, &texture)
    gl.BindTexture(gl.TEXTURE_2D, texture)

    gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT)
    gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT)
    gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST_MIPMAP_NEAREST)
    gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)

    gl.TexImage2D(gl.TEXTURE_2D, 0, gl.RGBA, width, height, 0, gl.RGBA, gl.UNSIGNED_BYTE, data)
    gl.GenerateMipmap(gl.TEXTURE_2D)

    stbi.image_free(data)

    return
}
