#version 330 core

in vec3 f_pos;
in vec3 f_nor;
in vec2 f_tex;

out vec4 FragColor;

uniform vec3 view_pos;

uniform sampler2D s_tex0;
uniform sampler2D s_tex1;

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dir_light;

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 attenuation;

    int is_spotlight;
    vec3 sl_direction;
    float sl_bright_rim;
    float sl_dark_rim;
};
#define LIGHTS_NUM 4
uniform Light lights[LIGHTS_NUM];

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

vec3 dir_light_calc(DirLight light, vec3 normal, vec3 view_dir)
{
    vec3 s2d_diff = vec3(texture(material.diffuse, f_tex));
    vec3 s2d_spec = vec3(texture(material.specular, f_tex));

    vec3 ambient = light.ambient * s2d_diff;
    
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(normal, light_dir), 0.0);

    vec3 diffuse = light.diffuse * diff * s2d_diff;

    vec3 reflect_dir = reflect(-light_dir, normal);
    float ispec = max(dot(view_dir, reflect_dir), 0.0);
    float spec = pow(ispec, material.shininess);

    vec3 specular = light.specular * spec * s2d_spec;

    return (diffuse + specular) + ambient;
}

vec3 light_calc(Light light, vec3 normal, vec3 view_dir)
{
    vec3 s2d_diff = vec3(texture(material.diffuse, f_tex));
    vec3 s2d_spec = vec3(texture(material.specular, f_tex));

    vec3 ambient = light.ambient * s2d_diff;

    vec3 light_dir = normalize(light.position - f_pos);
    float diff = max(dot(normal, light_dir), 0.0);

    vec3 diffuse = light.diffuse * diff * s2d_diff;

    vec3 reflect_dir = reflect(-light_dir, normal);
    float ispec = max(dot(view_dir, reflect_dir), 0.0);
    float spec = pow(ispec, material.shininess);

    vec3 specular = light.specular * spec * s2d_spec;

    float dist = length(light.position - f_pos);
    float luminosity = 1.0 / (
        light.attenuation[0] * dist * dist +
        light.attenuation[1] * dist +
        light.attenuation[2]
    );

    float intensity = 1;
    if (bool(light.is_spotlight))
    {
        float theta = dot(light_dir, normalize(-light.sl_direction));
        float epsilon = light.sl_bright_rim - light.sl_dark_rim;

        if (epsilon != 0.0) //gradual edge spotlight (I = Range[0,1])
        {
            float intensity_c = (theta - light.sl_dark_rim) / epsilon;
            intensity = clamp(intensity_c, 0.0, 1.0);
        }
        else //otherwise treat as using sharp edge spotlight (I = 1 or 0)
        {
            intensity = (theta > light.sl_bright_rim) ? 1.0 : 0.0;
        }
    }

    return intensity * luminosity * (diffuse + specular) + ambient;
}

void main()
{
    vec3 norm = normalize(f_nor);
    vec3 view_dir = normalize(view_pos - f_pos);

    vec3 light_result = dir_light_calc(dir_light, norm, view_dir);

    for (int i = 0; i < LIGHTS_NUM; i++)
    {
        light_result += light_calc(lights[i], norm, view_dir);
    }

    FragColor = vec4(light_result, 1.0);

    /* vec4 tex1 = texture(s_tex0, f_tex);
    vec4 tex2 = texture(s_tex1, f_tex);
    FragColor = mix(tex1, tex2, 0.2) * vec4(light_result, 1.0); */
    
} 