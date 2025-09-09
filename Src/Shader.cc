/**
 *   @file: Shader.cc
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include <fstream>

#include "Shader.hh"

Shader::Shader(string cref vert_path, string cref frag_path, int tex_units)
{
    int success;
    char info_log[512];

    optional<string> vert_code_str = get_file_contents(vert_path);
    if (vert_code_str == std::nullopt)
    {
        glfwTerminate();
        return;
    }
    char const* vert_code = vert_code_str.value().c_str();

    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_code, NULL);
    glCompileShader(vert_shader);
   
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
        print("ERROR: vert_shader compilation failed:\n{}\n", info_log);
        return;
    };

    optional<string> frag_code_str = get_file_contents(frag_path);
    if (frag_code_str == std::nullopt)
    {
        glfwTerminate();
        return;
    }
    char const* frag_code = frag_code_str.value().c_str();

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_code, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
        print("ERROR: frag_shader compilation failed:\n{}\n", info_log);
        return;
    };

    ID = glCreateProgram();
    glAttachShader(ID, vert_shader);
    glAttachShader(ID, frag_shader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, info_log);
        print("ERROR: shader linking failed:\n{}\n", info_log);
        return;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    //set indices for Sampler2D to use
    use();
    for (int i = 0; i < tex_units; i++)
        uniform_i(format("s_tex{}", i).c_str(), i);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::set_texture(int tex_unit, Texture cref texture) const
{
    glActiveTexture(GL_TEXTURE0 + tex_unit);
    glBindTexture(GL_TEXTURE_2D, texture.ID);
}

void Shader::uniform_f(string cref name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::uniform_i(string cref name, int value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::uniform_u(string cref name, uint value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
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

