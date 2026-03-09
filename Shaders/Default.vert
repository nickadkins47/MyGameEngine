#version 460 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_col;
layout (location = 2) in vec2 v_tex; //dummy, for now

out vec3 f_col;

void main()
{
    f_col = v_col;
    gl_Position = vec4(v_pos, 1.0);
}