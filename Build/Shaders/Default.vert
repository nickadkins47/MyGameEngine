#version 460 core

layout (location = 0) in vec3 v_in_pos;
layout (location = 1) in vec3 v_in_nor;
layout (location = 2) in vec2 v_in_tex;

out vert_data
{
    vec3 pos;
    vec3 nor;
    vec2 tex;
} v_out;

uniform mat4 vp_mat;
uniform mat4 m_mat;

//TODO: move some operations here to a compute shader thats done right before vertexes
//IE calc mvp_mat, n_mat (m_mat_itr), etc

void main()
{
    v_out.pos = vec3(m_mat * vec4(v_in_pos, 1.0));
    v_out.nor = mat3(transpose(inverse(m_mat))) * v_in_nor;
    v_out.tex = v_in_tex;
    gl_Position = vp_mat * m_mat * vec4(v_in_pos, 1.0);
}