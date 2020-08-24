#include "ResourceManager.h"

#include <glad/glad.h> 

std::unique_ptr<Shader>& ResourceManager::loadShader(std::string name, std::string path)
{
    mShaders[name] = std::make_unique<Shader>(path);
    return mShaders[name];
}

std::unique_ptr<CubeMapTexture>& ResourceManager::loadCubeMapTexture(std::string name, const std::vector<std::string>& paths)
{
    mCubeMapTextures[name] = std::make_unique<CubeMapTexture>(paths);
    return mCubeMapTextures[name];
}

std::unique_ptr<Texture>& ResourceManager::loadTexture(std::string name, std::string path)
{
    mTextures[name] = std::make_unique<Texture>(path);
    return mTextures[name];
}


std::unique_ptr<Shader>& ResourceManager::getShader(std::string name)
{
    return mShaders[name];
}

std::unique_ptr<CubeMapTexture>& ResourceManager::getCubeMapTexture(std::string name)
{
    return mCubeMapTextures[name];
}

std::unique_ptr<Texture>& ResourceManager::getTexture(std::string name)
{
    return mTextures[name];
}

void ResourceManager::clear()
{    
    for (auto& shader : mShaders)
    {
        glDeleteProgram(shader.second->getID());
    }

    for (auto& texture : mTextures)
    {
        unsigned int tex = texture.second->getID();
        glDeleteTextures(1, &tex);
    }

    for (auto& texture : mCubeMapTextures)
    {
        unsigned int tex = texture.second->getID();
        glDeleteTextures(1, &tex);
    }
}