#version 460 core
#extension GL_ARB_shader_draw_parameters : require

//Pass-Through Vertex Shader (important stuff is done in VoxQuads.geom)

layout (location = 0) in int v_in_data; //all data for one face

out vox_data
{
    int chk_data;
    int data;
} v_out;

void main()
{
    v_out.chk_data = gl_BaseInstance;
    v_out.data = v_in_data;
}