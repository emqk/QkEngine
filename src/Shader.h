#pragma once

#include <string>
#include <glm\ext\matrix_float4x4.hpp>

class Shader
{
public:
    unsigned int ID;

    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);

    std::string name;

    void Use();

    void SetVec4(const char* name, float r, float g, float b, float a) const;

    void SetMat4(const std::string& name, const glm::mat4& mat) const;

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;

private:
    void CheckCompileErrors(unsigned int shader, std::string type);
    std::string GetFileSource(const char* shaderPath);
};