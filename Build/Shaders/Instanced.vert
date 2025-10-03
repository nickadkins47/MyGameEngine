#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_nor;
layout (location = 2) in vec2 v_tex;
layout (location = 3) in mat4 m_mat; //div=1, loc=3,4,5,6

out vec3 f_pos;
out vec3 f_nor;
out vec2 f_tex;

uniform mat4 vp_mat;

//TODO: move some operations here to a compute shader thats done right before vertexes
//IE calc mvp_mat, m_mat_itr, etc

void main()
{
    f_pos = vec3(m_mat * vec4(v_pos, 1.0));
    f_nor = mat3(transpose(inverse(m_mat))) * v_nor;
    f_tex = v_tex;
    gl_Position = vp_mat * m_mat * vec4(v_pos, 1.0);
}
