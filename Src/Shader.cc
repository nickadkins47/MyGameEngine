/**
 *   @file: Shader.cc
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include "Ext/GL/Enum.hh"
#include "Ext/GL/Functions.hh"

#include "Engine.hh"
#include "Shader.hh"
#include "Texture.hh"

Shader::Shader() {}

optional<Shader ptr> Shader::add(string cref shader_path, int num_lights, int num_textures)
{
    Log::info("Adding shaders \"{}\"...", shader_path);

    int success;
    char info_log[512];

    optional<string> vert_code_ostr = get_file_contents(shader_path + ".vert");
    if (vert_code_ostr == nullopt)
    {
        Log::warn("Adding shaders \"{0}\": Failed (Cannot locate \"{0}.vert\")", shader_path);
        return nullopt;
    }
    string vert_code_str = vert_code_ostr.value();
    char const* vert_code = vert_code_str.data();

    uint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_code, NULL);
    glCompileShader(vert_shader);

    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
        Log::warn("Adding shaders \"{}\": Failed ({})", shader_path, info_log);
        return nullopt;
    }

    optional<string> frag_code_ostr = get_file_contents(shader_path + ".frag");
    if (frag_code_ostr == nullopt)
    {
        Log::warn("Adding shaders \"{0}\": Failed (Cannot locate \"{0}.frag\")", shader_path);
        return nullopt;
    }
    string frag_code_str = frag_code_ostr.value();
    char const* frag_code = frag_code_str.data();

    uint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_code, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
        Log::warn("Adding shaders \"{}\": Failed ({})", shader_path, info_log);
        return nullopt;
    }

    Shader ptr shader = get_new(shader_path);
    shader->num_textures = num_textures;

    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vert_shader);
    glAttachShader(shader->ID, frag_shader);
    glLinkProgram(shader->ID);

    glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shader->ID, 512, NULL, info_log);
        Log::warn("Adding shaders \"{}\": Failed ({})", shader_path, info_log);
        Shader::remove(shader_path);
        return nullopt;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    shader->use();
    for (int i = 0; i < num_lights; i++)
    {
        shader->uniform_i(format("lights[{}].mode", i), 0);
    }
    for (int i = 0; i < num_textures; i++)
    {
        shader->uniform_i(format("textures[{}].tex", i), i);
        shader->uniform_i(format("textures[{}].type", i), 0);
    }

    Log::info("Adding shaders \"{}\": Success", shader_path);
    return shader;
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::sampler2d(int tex_unit, Texture cptr texture) const
{
    glActiveTexture(GL_TEXTURE0 + tex_unit);
    glBindTexture(GL_TEXTURE_2D, texture->ID);
}

void Shader::uniform_f(string_view name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.data()), value);
}

void Shader::uniform_i(string_view name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.data()), value);
}

void Shader::uniform_u(string_view name, uint value) const
{
    glUniform1ui(glGetUniformLocation(ID, name.data()), value);
}

void Shader::uniform_fv(string_view name, int size, float cptr value) const
{
    if (size == 1)
    {
        glUniform1fv(glGetUniformLocation(ID, name.data()), 1, value);
    }
    else if (size == 2)
    {
        glUniform2fv(glGetUniformLocation(ID, name.data()), 1, value);
    }
    else if (size == 3)
    {
        glUniform3fv(glGetUniformLocation(ID, name.data()), 1, value);
    }
    else if (size == 4)
    {
        glUniform4fv(glGetUniformLocation(ID, name.data()), 1, value);
    }
}

void Shader::uniform_iv(string_view name, int size, int cptr value) const
{
    if (size == 1)
    {
        glUniform1iv(glGetUniformLocation(ID, name.data()), 1, value);
    }
    else if (size == 2)
    {
        glUniform2iv(glGetUniformLocation(ID, name.data()), 1, value);
    }
    else if (size == 3)
    {
        glUniform3iv(glGetUniformLocation(ID, name.data()), 1, value);
    }
    else if (size == 4)
    {
        glUniform4iv(glGetUniformLocation(ID, name.data()), 1, value);
    }
}

void Shader::uniform_uv(string_view name, int size, uint cptr value) const
{
    if (size == 1)
    {
        glUniform1uiv(glGetUniformLocation(ID, name.data()), 1, value);
    }
    else if (size == 2)
    {
        glUniform2uiv(glGetUniformLocation(ID, name.data()), 1, value);
    }
    else if (size == 3)
    {
        glUniform3uiv(glGetUniformLocation(ID, name.data()), 1, value);
    }
    else if (size == 4)
    {
        glUniform4uiv(glGetUniformLocation(ID, name.data()), 1, value);
    }
}

void Shader::uniform_fm(string_view name, int cols, int rows, float cptr value, bool transpose) const
{
    if (cols == 2)
    {
        if (rows == 2)
        {
            glUniformMatrix2fv  (glGetUniformLocation(ID, name.data()), 1, transpose, value);
        }
        else if (rows == 3)
        {
            glUniformMatrix2x3fv(glGetUniformLocation(ID, name.data()), 1, transpose, value);
        }
        else if (rows == 4)
        {
            glUniformMatrix2x4fv(glGetUniformLocation(ID, name.data()), 1, transpose, value);
        }
    }
    else if (cols == 3)
    {
        if (rows == 2)
        {
            glUniformMatrix3x2fv(glGetUniformLocation(ID, name.data()), 1, transpose, value);
        }
        else if (rows == 3)
        {
            glUniformMatrix3fv  (glGetUniformLocation(ID, name.data()), 1, transpose, value);
        }
        else if (rows == 4)
        {
            glUniformMatrix3x4fv(glGetUniformLocation(ID, name.data()), 1, transpose, value);
        }
    }
    else if (cols == 4)
    {
        if (rows == 2)
        {
            glUniformMatrix4x2fv(glGetUniformLocation(ID, name.data()), 1, transpose, value);
        }
        else if (rows == 3)
        {
            glUniformMatrix4x3fv(glGetUniformLocation(ID, name.data()), 1, transpose, value);
        }
        else if (rows == 4)
        {
            glUniformMatrix4fv  (glGetUniformLocation(ID, name.data()), 1, transpose, value);
        }
    }
}
