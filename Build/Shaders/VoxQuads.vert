#version 330 core

//Pass-Through Vertex Shader (important stuff is done in VoxQuads.geom)

layout (location = 0) in ivec3 v_in_pos; //x,y,z
layout (location = 1) in int v_in_face; //face index to render

out vox_data
{
    ivec3 pos;
    int face;
} v_out;

void main()
{
    v_out.pos = v_in_pos;
    v_out.face = v_in_face;
}