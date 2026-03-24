#version 460 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_col;
layout (location = 2) in vec2 v_tex;

out vec3 f_col;
out vec2 f_tex;

uniform mat4 m_mat;
uniform mat4 v_mat;
uniform mat4 p_mat;

void main()
{
    gl_Position = p_mat * v_mat * m_mat * vec4(v_pos, 1.0);
    f_col = v_col;
    f_tex = v_tex;
}