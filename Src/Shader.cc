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

bool Shader::add(string cref shader_name,
    string_view vert_path, string_view geom_path, string_view frag_path, int num_lights, int num_textures)
{
    Log log("Adding shaders \"{}\"", shader_name);

    optional<uint> vert_shader = add_one_shader(shader_name, vert_path, 0);
    if (vert_shader == nullopt) {
        log.fail("Cannot add vertex shader");
        return false;
    }

    optional<uint> geom_shader = add_one_shader(shader_name, geom_path, 1);
    if (geom_shader == nullopt) {
        log.fail("Cannot add geometry shader");
        return false;
    }

    optional<uint> frag_shader = add_one_shader(shader_name, frag_path, 2);
    if (frag_shader == nullopt) {
        log.fail("Cannot add fragment shader");
        return false;
    }
    
    Shader ptr shader = new_val(shader_name);
    shader->num_textures = num_textures;

    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vert_shader.value());
    //log.gl_check("Attaching Vert");
    glAttachShader(shader->ID, geom_shader.value());
    //log.gl_check("Attaching Geom");
    glAttachShader(shader->ID, frag_shader.value());
    //log.gl_check("Attaching Frag");
    glLinkProgram(shader->ID);
    //log.gl_check("Linking shader prog.");

    int success;
    char info_log[512];
    glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shader->ID, 512, NULL, info_log);
        log.fail("{}", info_log);
        Shader::remove(shader_name);
        return false;
    }

    glDeleteShader(vert_shader.value());
    glDeleteShader(geom_shader.value());
    glDeleteShader(frag_shader.value());

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

    return true;
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

optional<uint> Shader::add_one_shader(string_view shader_name, string_view sh_path, int mode)
{
    Log log("Adding shader part \"{}\"", sh_path);

    optional<string> sh_code_o = get_file_contents(sh_path);
    if (sh_code_o == nullopt)
    {
        log.fail("Cannot locate \"{}\"", sh_path);
        return nullopt;
    }
    char cptr sh_code = sh_code_o.value().data();

    GLenum type =
        (mode == 0) ? GL_VERTEX_SHADER :
        (mode == 1) ? GL_GEOMETRY_SHADER:
        /*(mode == 2) ?*/ GL_FRAGMENT_SHADER
    ;
    uint shader = glCreateShader(type);
    glShaderSource(shader, 1, &sh_code, NULL);
    glCompileShader(shader);

    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        log.fail("Cannot compile ({})", info_log);
        return nullopt;
    }

    return shader;
}