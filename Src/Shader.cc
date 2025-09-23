/**
 *   @file: Shader.cc
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include "Engine.hh"
#include "Shader.hh"

Shader::Shader() {}

optional<Shader ptr> Shader::add(path cref shader_p, int num_textures, int num_lights)
{
    Log::info("Adding shaders \"{}\"...", shader_p.string());

    int success;
    char info_log[512];
    path _shader_p (shader_p);

    _shader_p.replace_extension(".vert");
    optional<string> vert_code_ostr = get_file_contents(_shader_p);
    if (vert_code_ostr == nullopt)
    {
        Log::warn("Adding shaders \"{}\": Failed (Cannot locate \"{}\")",
            shader_p.string(), _shader_p.string()
        );
        return nullopt;
    }
    string vert_code_str = vert_code_ostr.value();
    char const* vert_code = vert_code_str.c_str();

    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_code, NULL);
    glCompileShader(vert_shader);
   
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
        Log::warn("Adding shaders \"{}\": Failed ({})", shader_p.string(), info_log);
        return nullopt;
    }

    _shader_p.replace_extension(".frag");
    optional<string> frag_code_ostr = get_file_contents(_shader_p);
    if (frag_code_ostr == nullopt)
    {
        Log::warn("Adding shaders \"{}\": Failed (Cannot locate \"{}\")",
            shader_p.string(), _shader_p.string()
        );
        return nullopt;
    }
    string frag_code_str = frag_code_ostr.value();
    char const* frag_code = frag_code_str.c_str();

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_code, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
        Log::warn("Adding shaders \"{}\": Failed ({})", shader_p.string(), info_log);
        return nullopt;
    }

    Shader ptr shader = &engine->shader_map[shader_p.string()];

    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vert_shader);
    glAttachShader(shader->ID, frag_shader);
    glLinkProgram(shader->ID);

    glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shader->ID, 512, NULL, info_log);
        Log::warn("Adding shaders \"{}\": Failed ({})", shader_p.string(), info_log);
        engine->shader_map.erase(shader_p.string());
        return nullopt;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    shader->use();
    for (int i = 0; i < num_textures; i++)
    {
        shader->uniform_i(format("textures[{}].tex", i), i);
        shader->uniform_i(format("textures[{}].type", i), 0);
    }

    shader->lights.reserve(num_lights);
    for (int i = 0; i < num_lights; i++)
    {
        shader->lights.emplace_back();
        shader->uniform_i(format("lights[{}].mode", i), 0);
    }

    Log::info("Adding shaders \"{}\": Success", shader_p.string());
    return shader;
}

optional<Shader ptr> Shader::get(string cref shader_name)
{
    Log::info("Getting shaders \"{}\"...", shader_name);
    auto iter = engine->shader_map.find(shader_name);
    if (iter == engine->shader_map.end())
    {
        Log::warn("Getting shaders \"{}\": Failed", shader_name);
        return nullopt;
    }
    else
    {
        Log::info("Getting shaders \"{}\": Success", shader_name);
        return &iter->second;
    }
}

bool Shader::exists(string cref shader_name)
{
    return engine->shader_map.contains(shader_name);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::sampler2d(int tex_unit, Texture cref texture) const
{
    glActiveTexture(GL_TEXTURE0 + tex_unit);
    glBindTexture(GL_TEXTURE_2D, texture.ID);
}

void Shader::update_light(int index) const
{
    Light cref light = lights.at(index);
    string const lname = format("lights[{}].", index);
    uniform_i(lname+"mode", light.mode);
    uniform_fv(lname+"diffuse", 3, glm::value_ptr(light.diffuse));
    uniform_fv(lname+"specular", 3, glm::value_ptr(light.specular));
    uniform_fv(lname+"ambient", 3, glm::value_ptr(light.ambient));
    uniform_fv(lname+"attenuation", 3, glm::value_ptr(light.attenuation));
    uniform_fv(lname+"position", 3, glm::value_ptr(light.position));
    uniform_fv(lname+"direction", 3, glm::value_ptr(light.direction));
    uniform_f(lname+"bright_rim", light.bright_rim);
    uniform_f(lname+"dark_rim", light.dark_rim);
}

void Shader::update_light_pos(int index) const
{
    uniform_fv(format("lights[{}].position", index), 3, glm::value_ptr(lights.at(index).position));
}

void Shader::uniform_f(string cref name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::uniform_i(string cref name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::uniform_u(string cref name, uint value) const
{
    glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::uniform_fv(string cref name, int size, float cptr value) const
{
    if (size == 1)
    {
        glUniform1fv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    else if (size == 2)
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    else if (size == 3)
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    else if (size == 4)
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
}

void Shader::uniform_iv(string cref name, int size, int cptr value) const
{
    if (size == 1)
    {
        glUniform1iv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    else if (size == 2)
    {
        glUniform2iv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    else if (size == 3)
    {
        glUniform3iv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    else if (size == 4)
    {
        glUniform4iv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
}

void Shader::uniform_uv(string cref name, int size, uint cptr value) const
{
    if (size == 1)
    {
        glUniform1uiv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    else if (size == 2)
    {
        glUniform2uiv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    else if (size == 3)
    {
        glUniform3uiv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    else if (size == 4)
    {
        glUniform4uiv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
}

void Shader::uniform_fm(string cref name, int cols, int rows, float cptr value, bool transpose) const
{
    if (cols == 2)
    {
        if (rows == 2)
        {
            glUniformMatrix2fv  (glGetUniformLocation(ID, name.c_str()), 1, transpose, value);
        }
        else if (rows == 3)
        {
            glUniformMatrix2x3fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, value);
        }
        else if (rows == 4)
        {
            glUniformMatrix2x4fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, value);
        }
    }
    else if (cols == 3)
    {
        if (rows == 2)
        {
            glUniformMatrix3x2fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, value);
        }
        else if (rows == 3)
        {
            glUniformMatrix3fv  (glGetUniformLocation(ID, name.c_str()), 1, transpose, value);
        }
        else if (rows == 4)
        {
            glUniformMatrix3x4fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, value);
        }
    }
    else if (cols == 4)
    {
        if (rows == 2)
        {
            glUniformMatrix4x2fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, value);
        }
        else if (rows == 3)
        {
            glUniformMatrix4x3fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, value);
        }
        else if (rows == 4)
        {
            glUniformMatrix4fv  (glGetUniformLocation(ID, name.c_str()), 1, transpose, value);
        }
    }
}

