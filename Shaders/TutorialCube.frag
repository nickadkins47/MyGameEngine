#version 330 core

in vec3 f_pos;
in vec3 f_nor;
in vec2 f_tex;

out vec4 FragColor;

uniform vec3 view_pos;

uniform sampler2D s_tex0;
uniform sampler2D s_tex1;

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

void main()
{
    vec3 s2d_diff = vec3(texture(material.diffuse, f_tex));
    vec3 s2d_spec = vec3(texture(material.specular, f_tex));

    vec3 ambient = light.ambient * s2d_diff;

    vec3 norm = normalize(f_nor);
    vec3 light_dir = normalize(light.position - f_pos);
    float diff = max(dot(norm, light_dir), 0.0);

    vec3 diffuse = light.diffuse * diff * s2d_diff;

    vec3 view_dir = normalize(view_pos - f_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float ispec = max(dot(view_dir, reflect_dir), 0.0);
    float spec = pow(ispec, material.shininess);

    vec3 specular = light.specular * spec * s2d_spec;

    vec3 light_result = ambient + diffuse + specular;

    /* vec4 tex1 = texture(s_tex0, f_tex);
    vec4 tex2 = texture(s_tex1, f_tex);
    FragColor = mix(tex1, tex2, 0.2) * vec4(light_result, 1.0); */

    FragColor = vec4(light_result, 1.0);
} 