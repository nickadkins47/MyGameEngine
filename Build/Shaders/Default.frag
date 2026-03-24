#version 460 core

in vec3 f_col;
in vec2 f_tex;

out vec4 f_out_color;

uniform sampler2D tex1;

void main()
{
    f_out_color = texture(tex1, f_tex);// * vec4(f_col, 1.0);
}