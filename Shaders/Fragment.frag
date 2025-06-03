#version 330 core

in vec3 f_color;
in vec2 f_texco;

out vec4 FragColor;

uniform sampler2D s_tex0;
uniform sampler2D s_tex1;

void main()
{
    //FragColor = vec4(f_color, 1.0);
    //FragColor = texture(our_texture, f_texco) * vec4(f_color, 1.0);
    FragColor = mix(texture(s_tex0, f_texco), texture(s_tex1, f_texco), 0.2);
} 