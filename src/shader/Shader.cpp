#include "shader.h"
#include <glad/glad.h> 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filePath)
    :mFilePath(filePath), mShaderID(0)
{

    //ShaderProgramSource source = parseShader("res/shaders/basic.shader");
    ShaderProgramSource source = parseShader(filePath);
    //std::cout << "Vertex\n" << source.vertexSource << "\n";
    //std::cout << "Fragment\n" << source.fragmentSource << "\n";

    mShaderID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(mShaderID);
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();

    unsigned int vShader = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //string needs to alive at this point
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); //adds onto the stack dynamically
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile: " <<
            (type == GL_VERTEX_SHADER ? "Vertex " : "Shader ")
            << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

ShaderProgramSource Shader::parseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);


    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    if (!stream)
    {
        std::cout << filePath << " not found!!\n";
    }
    else
    {
        std::cout << filePath << " found!!\n";
    }
    while (getline(stream, line))
    {
        //std::cout << line << "\n";
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

void Shader::bind() const
{
    glUseProgram(mShaderID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform1i(const std::string& name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform1f(const std::string& name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformVec2f(const std::string& name, const glm::vec2& vec)
{
    glUniform2f(getUniformLocation(name), vec.x, vec.y);
}

void Shader::setUniformVec3f(const std::string& name, const glm::vec3& vec)
{
    glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
}

void Shader::setUniformVec4f(const std::string& name, const glm::vec4& vec)
{
    glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniformArray1f(const std::string& name, unsigned int count, const float array[])
{
    glUniform1fv(getUniformLocation(name), 9, array); //todo, why is this hardcoded???
}

void Shader::setUniformArray2f(const std::string& name, unsigned int count, const float array[][2])
{
    glUniform2fv(getUniformLocation(name), count, *array);
}

void Shader::setUniformArray1v(const std::string& name, unsigned int count, const int array[])
{
    glUniform1iv(getUniformLocation(name), count, array);
}


int Shader::getUniformLocation(const std::string& name) const
{
    if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
    {
        return mUniformLocationCache[name];
    }

    int location = glGetUniformLocation(mShaderID, name.c_str()); //-1 is an error
    if (location == -1)
    {
        //if a uniform isn't used it won't be found
        std::cout << name.c_str() << " location not set in shader\n";
    }
    else
    {        
        //std::cout << name.c_str() << " location: " << location << "\n";
    }
    mUniformLocationCache[name] = location;


    return location;
}