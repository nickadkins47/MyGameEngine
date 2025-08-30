#version 330 core

in vec2 f_texco;

out vec4 FragColor;

uniform sampler2D s_tex0;

void main()
{
    FragColor = texture(s_tex0, f_texco);
} 