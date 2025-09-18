#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_nor;
layout (location = 2) in vec2 v_tex;

out vec3 f_pos;
out vec3 f_nor;
out vec2 f_tex;

uniform mat4 mvp_mat;
uniform mat4 m_mat;
uniform mat3 m_mat_itr;

void main()
{
    f_pos = vec3(m_mat * vec4(v_pos, 1.0));
    f_nor = m_mat_itr * v_nor;
    f_tex = v_tex;
    gl_Position = mvp_mat * vec4(v_pos, 1.0);
}