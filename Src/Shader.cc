/**
 *   @file: Shader.cc
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include "Shader.hh"

Shader::Shader(string const& vert_path, string const& frag_path)
{
    int success;
    char info_log[512];

    std::ifstream vert_file(get_file(vert_path));
    if (vert_file.fail())
    {
        print("Wrong file dude (vert)\n");
        glfwTerminate();
        return;
    }
    std::string vert_code_str = (std::stringstream() << vert_file.rdbuf()).str();
    char const* vert_code = vert_code_str.c_str();
    vert_file.close();

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

    std::ifstream frag_file(get_file(frag_path));
    if (frag_file.fail())
    {
        print("Wrong file dude (frag)\n");
        glfwTerminate();
        return;
    }
    std::string frag_code_str = (std::stringstream() << frag_file.rdbuf()).str();
    char const* frag_code = frag_code_str.c_str();
    frag_file.close();

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

    id = glCreateProgram();
    glAttachShader(id, vert_shader);
    glAttachShader(id, frag_shader);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, info_log);
        print("ERROR: shader linking failed:\n{}\n", info_log);
        return;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

void Shader::use() const
{
    glUseProgram(id);
}

void Shader::set_bool(string const& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void Shader::set_int(string const& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set_float(string const& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}