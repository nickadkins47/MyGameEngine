#version 330 core

in vec3 f_pos;
in vec3 f_nor;
in vec2 f_tex;

out vec4 out_color;

struct Light
{
    int mode; //0: Ignore, 1: Normal Light, 2: Global Light, 3: Spotlight

    vec3 ambient;     // n g s
    vec3 diffuse;     // n g s
    vec3 specular;    // n g s
    vec3 attenuation; // n   s
    vec3 position;    // n   s
    vec3 direction;   //   g s
    float bright_rim; //     s
    float dark_rim;   //     s
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
vec3 view_dir;
vec3 normal;

vec3 s2d_diff;
vec3 s2d_spec;

float calc_luminosity(Light light)
{
    float dist = length(light.position - f_pos);
    return 1.0 / (
        light.attenuation[0] * dist * dist +
        light.attenuation[1] * dist +
        light.attenuation[2]
    );
}

float calc_intensity(Light light, vec3 light_dir)
{
    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.bright_rim - light.dark_rim;

    if (epsilon != 0.0) //gradual edge spotlight (I = Range[0,1])
    {
        return clamp((theta - light.dark_rim) / epsilon, 0.0, 1.0);
    }
    else //sharp edge spotlight (I = 1 or 0)
    {
        return (theta > light.bright_rim) ? 1.0 : 0.0;
    }
}

vec3 light_calc(Light light)
{
    vec3 ambient = light.ambient * s2d_diff;

    vec3 light_dir = (light.mode == 2)
        ? normalize(-light.direction) //global light
        : normalize(light.position - f_pos) //spot/normal light
    ;
    float diff = max(dot(normal, light_dir), 0.0);

    vec3 diffuse = light.diffuse * diff * s2d_diff;

    vec3 reflect_dir = reflect(-light_dir, normal);
    float ispec = max(dot(view_dir, reflect_dir), 0.0);
    float spec = pow(ispec, material.shininess);

    vec3 specular = light.specular * spec * s2d_spec;

    //If light isnt a global light, then calc luminosity
    float luminosity = (light.mode != 2)
        ? calc_luminosity(light)
        : 1.0
    ;

    //If light is a spotlight, then calc intensity
    float intensity = (light.mode == 3)
        ? calc_intensity(light, light_dir)
        : 1.0
    ;
    
    return intensity * luminosity * (diffuse + specular) + ambient;
}

void main()
{
    view_dir = normalize(view_pos - f_pos);
    normal = normalize(f_nor);

    //TODO: dynamically handle textures
    s2d_diff = vec3(texture(material.diffuse, f_tex));
    s2d_spec = vec3(texture(material.specular, f_tex));

    vec3 light_result = vec3(0.0);

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