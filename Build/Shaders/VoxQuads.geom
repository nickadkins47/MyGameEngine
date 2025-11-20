#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in vox_data
{
    int chk_data;
    int data; //all data for one face
} g_in[];

out vert_data
{
    vec3 pos;
    vec3 nor;
    vec2 tex;
} g_out;

const vec3 v[8] = vec3[](
    vec3(0.0, 0.0, 0.0), //0
    vec3(0.0, 0.0, 1.0), //1
    vec3(0.0, 1.0, 0.0), //2
    vec3(0.0, 1.0, 1.0), //3
    vec3(1.0, 0.0, 0.0), //4
    vec3(1.0, 0.0, 1.0), //5
    vec3(1.0, 1.0, 0.0), //6
    vec3(1.0, 1.0, 1.0)  //7
);

const vec3 n[6] = vec3[](
    vec3(-1.0, 0.0, 0.0), //nx
    vec3( 1.0, 0.0, 0.0), //px
    vec3( 0.0,-1.0, 0.0), //ny
    vec3( 0.0, 1.0, 0.0), //py
    vec3( 0.0, 0.0,-1.0), //nz
    vec3( 0.0, 0.0, 1.0)  //pz
);

uniform mat4 vp_mat;
uniform int vox_x_dim;
uniform int vox_y_dim;

int face;

ivec3 bpos; //block position (relative to chunk)
ivec3 cpos; //chunk position (relative to world)

void make_vertex(int v_i, vec2 tex)
{
    vec3 vpos = v[v_i]; //vertex position (relative to block)
    vec3 pos = vpos + bpos + cpos;
    gl_Position = vp_mat * vec4(pos, 1.0);
    g_out.pos = pos;
    g_out.nor = n[face];
    g_out.tex = tex;
    EmitVertex();
}

void make_face(int i0, int i1, int i2, int i3)
{
    make_vertex(i0, vec2(1.0,0.0));
    make_vertex(i1, vec2(1.0,1.0));
    make_vertex(i2, vec2(0.0,1.0));
    EndPrimitive();
    make_vertex(i2, vec2(0.0,1.0));
    make_vertex(i3, vec2(0.0,0.0));
    make_vertex(i0, vec2(1.0,0.0));
    EndPrimitive();
}

void main()
{
    int cx = (g_in[0].chk_data >> 16) & 65535;
    int cy = (g_in[0].chk_data >> 0 ) & 65535;
    cpos = ivec3(cx * vox_x_dim, cy * vox_y_dim, 1);

    int data = g_in[0].data;
    face  = (data >> 15) & 7 ;
    int x = (data >> 10) & 31;
    int y = (data >> 5 ) & 31;
    int z = (data >> 0 ) & 31;
    bpos = ivec3(x,y,z);

    switch (face)
    {
        case 0: make_face(0,1,3,2); break;
        case 1: make_face(6,7,5,4); break;
        case 2: make_face(4,5,1,0); break;
        case 3: make_face(2,3,7,6); break;
        case 4: make_face(0,2,6,4); break;
        case 5: make_face(5,7,3,1); break;
    }
    /*
        (0.0, 0.0, 0.0), //0
        (0.0, 0.0, 1.0), //1
        (0.0, 1.0, 0.0), //2
        (0.0, 1.0, 1.0), //3
        (1.0, 0.0, 0.0), //4
        (1.0, 0.0, 1.0), //5
        (1.0, 1.0, 0.0), //6
        (1.0, 1.0, 1.0)  //7
    */
}