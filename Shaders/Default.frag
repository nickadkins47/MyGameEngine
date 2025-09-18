#version 330 core

in vec3 f_pos;
in vec3 f_nor;
in vec2 f_tex;

out vec4 out_color;

struct DirLight
{
    int mode; //0: Ignore, 1: Normal Light

    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define DIR_LIGHTS_NUM 1
uniform DirLight dir_lights[DIR_LIGHTS_NUM];

struct Light
{
    int mode; //0: Ignore, 1: Normal Light, 2: Spotlight

    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
    vec3 sl_direction;
    float sl_bright_rim;
    float sl_dark_rim;
};
#define LIGHTS_NUM 8
uniform Light lights[LIGHTS_NUM];

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

uniform sampler2D s_tex0;
uniform sampler2D s_tex1;

uniform vec3 view_pos;

vec3 normal;
vec3 view_dir;
vec3 s2d_diff;
vec3 s2d_spec;

vec3 dir_light_calc(DirLight light)
{
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

vec3 light_calc(Light light)
{
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
    if (light.mode == 2)
    {
        float theta = dot(light_dir, normalize(-light.sl_direction));
        float epsilon = light.sl_bright_rim - light.sl_dark_rim;

        if (epsilon != 0.0) //gradual edge spotlight (I = Range[0,1])
        {
            intensity = clamp((theta - light.sl_dark_rim) / epsilon, 0.0, 1.0);
        }
        else //sharp edge spotlight (I = 1 or 0)
        {
            intensity = (theta > light.sl_bright_rim) ? 1.0 : 0.0;
        }
    }

    return intensity * luminosity * (diffuse + specular) + ambient;
}

void main()
{
    normal = normalize(f_nor);
    view_dir = normalize(view_pos - f_pos);
    s2d_diff = vec3(texture(material.diffuse, f_tex));
    s2d_spec = vec3(texture(material.specular, f_tex));

    vec3 light_result = vec3(0.0);
    
    for (int i = 0; i < DIR_LIGHTS_NUM; i++)
    {
        if (dir_lights[i].mode != 0)
            light_result += dir_light_calc(dir_lights[i]);
    }

    for (int i = 0; i < LIGHTS_NUM; i++)
    {
        if (lights[i].mode != 0)
            light_result += light_calc(lights[i]);
    }

    out_color = vec4(light_result, 1.0);

    /* vec4 tex1 = texture(s_tex0, f_tex);
    vec4 tex2 = texture(s_tex1, f_tex);
    out_color = mix(tex1, tex2, 0.2) * vec4(light_result, 1.0); */
    
} 