#pragma once
#include <string>
#include <unordered_map>

#include "glm.hpp"

#include <iostream>
#include <glad/glad.h> 

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

enum class ShaderType
{
	NONE = -1,
	VERTEX = 0,
	FRAGMENT = 1
};

class Shader
{
private:
	std::string mFilePath;

	unsigned int mShaderID;
	//uniform cache
	mutable std::unordered_map<std::string, int> mUniformLocationCache;

	int getUniformLocation(const std::string& name) const;

	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int compileShader(unsigned int type, const std::string& source);
	ShaderProgramSource parseShader(const std::string& filePath);
	
public:
	Shader(const std::string& filePath);
	~Shader();

	void bind() const;
	void unbind() const;

	const unsigned int getID() const { return mShaderID; };

	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);

	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void setUniformVec2f(const std::string& name, const glm::vec2& vec);
	void setUniformVec3f(const std::string& name, const glm::vec3& vec);
	void setUniformVec4f(const std::string& name, const glm::vec4& vec);

	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

	void setUniformArray1f(const std::string& name, unsigned int count, const float array[]);
	void setUniformArray2f(const std::string& name, unsigned int count, const float array[][2]);

	void setUniformArray1v(const std::string& name, unsigned int count, const int array[]);

};