#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>


Shader::Shader()
{
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexShaderSource = GetFileSource(vertexPath);
    const char* vShaderCode = vertexShaderSource.c_str();
    
    std::string fragmentShaderSource = GetFileSource(fragmentPath);
    const char* fShaderCode = fragmentShaderSource.c_str();
    
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() const
{
    glUseProgram(ID);
}

void Shader::SetVec2(const char* name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name), x, y);
}

void Shader::SetVec3(const char* name, float r, float g, float b) const
{
    glUniform3f(glGetUniformLocation(ID, name), r, g, b);
}

void Shader::SetVec4(const char* name, float r, float g, float b, float a) const
{
    glUniform4f(glGetUniformLocation(ID, name), r, g, b, a);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

std::string Shader::GetFileSource(const char* shaderPath)
{
    std::string str;
    std::string line;
    std::ifstream shaderFile(shaderPath);
    if (shaderFile.is_open())
    {
        while (getline(shaderFile, line))
        {
            str += line + "\n";
        }
        shaderFile.close();
    }
    else
    {
        std::cout << "Can't find shader! - " << shaderPath << "\n";
    }

    return str;
}
