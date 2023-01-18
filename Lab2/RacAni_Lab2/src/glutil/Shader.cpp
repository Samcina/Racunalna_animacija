#include "Shader.h"
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexFilePath, const std::string& geometryFilePath, const std::string& fragmentFilePath)
	:
	vertexFilePath(vertexFilePath),
    geometryFilePath(geometryFilePath),
	fragmentFilePath(fragmentFilePath),
	id(0)
{
    std::string vertexShaderSrc = LoadShader(vertexFilePath);
    std::string geometryShaderSrc = LoadShader(geometryFilePath);
    std::string fragmentShaderSrc = LoadShader(fragmentFilePath);
    id = CreateShader(vertexShaderSrc, geometryShaderSrc, fragmentShaderSrc);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::Bind() const
{
    glUseProgram(id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat4)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::SetUniformVec4f(const std::string& name, const glm::vec4& vec4)
{
    glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vec4));
}

std::string Shader::LoadShader(const std::string& filePath)
{
    std::ifstream fs(filePath);
    std::string line;
    std::stringstream ss;

    while (getline(fs, line))
    {
        ss << line << "\n";
    }

    return ss.str();
}

unsigned int Shader::CreateShader(const std::string& vertexShaderSrc, const std::string& geometryShaderSrc, const std::string& fragmentShaderSrc)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    unsigned int gs = CompileShader(GL_GEOMETRY_SHADER, geometryShaderSrc);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(gs);
    glDeleteShader(fs);

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : (GL_GEOMETRY_SHADER ? "geometry" : "fragment" )) << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];

    int location = glGetUniformLocation(id, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
   
    uniformLocationCache[name] = location;
    return location;
}
