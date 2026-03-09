#version 460 core

in vec3 f_col;

out vec4 f_out_color;

void main()
{
    f_out_color = vec4(f_col, 1.0);
}