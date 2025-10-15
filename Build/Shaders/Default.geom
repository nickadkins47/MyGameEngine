#version 330 core

//Pass-through Geometry Shader for Triangles

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vert_data
{
    vec3 pos;
    vec3 nor;
    vec2 tex;
} g_in[];

out vert_data
{
    vec3 pos;
    vec3 nor;
    vec2 tex;
} g_out;

void make_vertex(int index)
{
    gl_Position = gl_in[index].gl_Position;
    g_out.pos = g_in[index].pos;
    g_out.nor = g_in[index].nor;
    g_out.tex = g_in[index].tex;
    EmitVertex();
}

void main()
{
    for (int i = 0; i < 3; i++)
        make_vertex(i);
    EndPrimitive();
}