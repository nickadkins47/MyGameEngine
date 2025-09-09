#version 330 core

in vec3 f_col;
in vec2 f_tex;

out vec4 FragColor;

uniform sampler2D s_tex0;

void main()
{
    FragColor = texture(s_tex0, f_tex) * vec4(f_col, 1.0);
} 