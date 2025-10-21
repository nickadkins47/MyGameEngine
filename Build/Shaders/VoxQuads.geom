#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in vox_data
{
    ivec3 pos; //x,y,z
    int face; //face index to render
} g_in[];

out vert_data
{
    vec3 pos;
    vec3 nor;
    vec2 tex;
} g_out;

uniform mat4 vp_mat;

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

void make_vertex(int v_i, vec2 tex)
{
    vec3 pos = g_in[0].pos + v[v_i];
    gl_Position = vp_mat * vec4(pos, 1.0);
    g_out.pos = pos;
    g_out.nor = n[g_in[0].face];
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
    switch (g_in[0].face)
    {
        case 0: make_face(0,1,3,2); break;
        case 1: make_face(6,7,5,4); break;
        case 2: make_face(4,5,1,0); break;
        case 3: make_face(2,3,7,6); break;
        case 4: make_face(0,2,6,4); break;
        case 5: make_face(5,7,3,1); break;
    }
    /*
        vec3(0.0, 0.0, 0.0), //0
        vec3(0.0, 0.0, 1.0), //1
        vec3(0.0, 1.0, 0.0), //2
        vec3(0.0, 1.0, 1.0), //3
        vec3(1.0, 0.0, 0.0), //4
        vec3(1.0, 0.0, 1.0), //5
        vec3(1.0, 1.0, 0.0), //6
        vec3(1.0, 1.0, 1.0)  //7
    */
}