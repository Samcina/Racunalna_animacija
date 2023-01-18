#pragma once
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>


class Shader
{
public:
	Shader(const std::string& vertexFilePath, const std::string& geometryFilePath, const std::string& fragmentFilePath);
	Shader() = default;
	~Shader();

	void Bind() const;
	void Unbind() const;
public:
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat4);
	void SetUniformVec4f(const std::string& name, const glm::vec4& vec4);
private:
	std::string LoadShader(const std::string& filePath);
	unsigned int CreateShader(const std::string& vertexShaderSrc, const std::string& geometryShaderSrc, const std::string& fragmentShaderSrc);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int GetUniformLocation(const std::string& name);
private:
	unsigned int id;
	std::string vertexFilePath;
	std::string geometryFilePath;
	std::string fragmentFilePath;
	std::unordered_map<std::string, int> uniformLocationCache;
};