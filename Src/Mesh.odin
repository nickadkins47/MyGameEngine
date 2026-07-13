package MyGameEngine

import gl "vendor:OpenGL"

//A vertex containing Position/Normal/Texture coordinates
Vertex :: struct
{
    x, y, z : f32, //Position
    nx,ny,nz: f32, //Normals
    tx,ty   : f32  //TexCoords
}

//Vertex Attributes for the above Vertex struct
DefaultVertexAttribs :: []VertexAttribDesc{
    {3, .FLOAT}, {3, .FLOAT}, {2, .FLOAT}
}

Mesh :: struct
{
    VAO : u32, //Vertex Array Object
    VBO : u32, //Vertex Buffer Object (Main)
    //IVBO: u32, //Vertex Buffer Object (Instances) //TODO
    EBO : u32, //Element Buffer Object

    vertices : [dynamic]Vertex,
    indices  : [dynamic]u32,
    //textures : [dynamic]^u32, //TODO

    prim_type : u32, //primitive type: TRIANGLE_STRIP, LINES, etc
}

//Currently empty proc that returns blank mesh
//
//Add your vertices & indices in manually, and once youre done call
//"mesh_finalize_gl_data" to create the buffer objects etc
mesh_add :: proc() -> (mesh: Mesh)
{
    return
}

mesh_delete :: proc(mesh: ^Mesh)
{
    gl.DeleteBuffers(1, &mesh.VBO)
    gl.DeleteBuffers(1, &mesh.VAO)

    delete(mesh.vertices)
    delete(mesh.indices)
}

mesh_finalize_gl_data :: proc(
    mesh: ^Mesh,
    prim_type: u32,
    attribs: []VertexAttribDesc = DefaultVertexAttribs,
    is_indexed: bool = false,
)
{
    gl.GenVertexArrays(1, &mesh.VAO)
	gl.BindVertexArray(mesh.VAO)

	gl.GenBuffers(1, &mesh.VBO)
	gl.BindBuffer(gl.ARRAY_BUFFER, mesh.VBO)
    set_vertex_attributes(..attribs)

    gl.BufferData(
        gl.ARRAY_BUFFER,
        len(mesh.vertices) * size_of(Vertex),
        raw_data(mesh.vertices),
        gl.STATIC_DRAW
    )

    if is_indexed {
        gl.GenBuffers(1, &mesh.EBO)
        gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, mesh.EBO)
        gl.BufferData(
            gl.ELEMENT_ARRAY_BUFFER,
            len(mesh.indices) * size_of(u32),
            raw_data(mesh.indices),
            gl.STATIC_DRAW
        )
    }

    mesh.prim_type = prim_type
}

mesh_render :: proc(mesh: ^Mesh)
{
    gl.BindVertexArray(mesh.VAO)
    if mesh.EBO != 0 { //If Indexed
        gl.DrawElements(mesh.prim_type, i32(len(mesh.indices)), gl.UNSIGNED_INT, nil)
    } else {
        gl.DrawArrays(mesh.prim_type, 0, i32(len(mesh.vertices)))
    }
}

VertexAttribType :: enum {
    //Integers
    //BYTE   = gl.BYTE,           //8b
    //UBYTE  = gl.UNSIGNED_BYTE,
    //SHORT  = gl.SHORT,          //16b
    //USHORT = gl.UNSIGNED_SHORT,
    INT    = gl.INT,            //32b
    UINT   = gl.UNSIGNED_INT,

    //Floating Point
    //HALF   = gl.HALF_FLOAT,     //16b
    FLOAT  = gl.FLOAT,          //32b
    DOUBLE = gl.DOUBLE,         //64b

    //Fixed Point
    //FIXED  = gl.FIXED,          //16b/16b

    //Boolean
    BOOL   = gl.BOOL,           //boolean
}

VertexAttribDesc :: struct
{
    size : i32,
    type : VertexAttribType
}

get_sizeof_attribute :: proc(attr: VertexAttribDesc) -> i32
{
    switch attr.type {
        case .INT   : return attr.size * size_of(i32)
        case .UINT  : return attr.size * size_of(u32)
        case .FLOAT : return attr.size * size_of(f32)
        case .DOUBLE: return attr.size * size_of(f64)
        case .BOOL  : return attr.size * size_of(bool)
    }
    return 0 //<- compiler complains otherwise
}

set_vertex_attributes :: proc(attribs: ..VertexAttribDesc)
{
    stride : i32 = 0
    for attr in attribs {
        stride += get_sizeof_attribute(attr)
    }

    pointer : uintptr = 0
    for attr, index in attribs {
        gl.VertexAttribPointer(
            u32(index), attr.size, u32(attr.type), false, stride, pointer
        )
        gl.EnableVertexAttribArray(u32(index))
        pointer += uintptr(get_sizeof_attribute(attr))
    }
}