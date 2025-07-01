#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texco;

out vec3 f_color;
out vec2 f_texco;

uniform mat4 mvp_mat;

void main()
{
    f_color = v_color;
    f_texco = v_texco;
    gl_Position = mvp_mat * vec4(v_pos, 1.0);
}