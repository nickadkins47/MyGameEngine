package MyGameEngine

import "core:fmt"
import "core:strings"

import gl "vendor:OpenGL"
import stbi "vendor:stb/image"

//2D Texture (loaded from a jpg, png, etc.)
Texture :: struct
{
    id: u32,
    type: TextureType
}

//List of supported Texture types
TextureType :: enum u32
{
	NONE         = 0x0,
	DIFFUSE      = 0x1,
	SPECULAR     = 0x2,
	AMBIENT      = 0x3,
	//EMISSIVE     = 0x4,
	//HEIGHT       = 0x5,
	//NORMALS      = 0x6,
	//SHININESS    = 0x7,
	//OPACITY      = 0x8,
	//DISPLACEMENT = 0x9,
	//LIGHTMAP     = 0xA,
	//REFLECTION   = 0xB,
	UNKNOWN      = 0xC,
}


texture_add :: proc(tex_path: string) -> (texture: Texture)
{
    stbi.flip_vertically_on_write(true)

    width, height, src_channels: i32
    
    data := stbi.load(strings.clone_to_cstring(tex_path), &width, &height, &src_channels, 4)
    if data == nil {
        fmt.panicf("Cannot load texture %v", tex_path)
    }

    gl.GenTextures(1, &texture.id)
    gl.BindTexture(gl.TEXTURE_2D, texture.id)

    gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT)
    gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT)
    gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST_MIPMAP_NEAREST)
    gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)

    gl.TexImage2D(gl.TEXTURE_2D, 0, gl.RGBA, width, height, 0, gl.RGBA, gl.UNSIGNED_BYTE, data)
    gl.GenerateMipmap(gl.TEXTURE_2D)

    stbi.image_free(data)

    return
}

texture_bind :: proc(texture: Texture, slot: u32)
{
    gl.ActiveTexture(gl.TEXTURE0 + slot)
	gl.BindTexture(gl.TEXTURE_2D, texture.id)
}
