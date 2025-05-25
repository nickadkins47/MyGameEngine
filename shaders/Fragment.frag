#version 460

layout (location = 0) in vec4 v_color;
layout (location = 0) out vec4 f_color;

layout (std140, set = 3, binding = 0) uniform UniformBlock {
    float time;
};

void main()
{
    float pulse = sin(time * 2.0) * 0.5 + 0.5;
    f_color = vec4(v_color.rgb * (0.8 + pulse * 0.5), v_color.a);
    //f_color = vec4(vec3(gl_FragCoord.z), 1.0);
    //f_color = v_color;
}