#pragma once

#include "Shader.h"
#include "CubeMapTexture.h"
#include "Texture.h"

#include <map>
#include <string>
#include <memory>
#include <vector>

class ResourceManager
{
private:

	std::map<std::string, std::unique_ptr<Shader>> mShaders;
	std::map<std::string, std::unique_ptr<CubeMapTexture>> mCubeMapTextures;
	std::map<std::string, std::unique_ptr<Texture>> mTextures;
	
public:	

	std::unique_ptr<Shader>& loadShader(std::string name, std::string path);
	std::unique_ptr<CubeMapTexture>& loadCubeMapTexture(std::string name, const std::vector<std::string>& paths);
	std::unique_ptr<Texture>& loadTexture(std::string name, std::string path);

	std::unique_ptr<Shader>& getShader(std::string name);
	std::unique_ptr<CubeMapTexture>& getCubeMapTexture(std::string name);
	std::unique_ptr<Texture>& getTexture(std::string name);

	const unsigned int getShaderID(std::string name)
	{
		return mShaders[name]->getID();
	}

	const unsigned int getCubeMapTextureID(std::string name)
	{
		return mCubeMapTextures[name]->getID();
	}

	const unsigned int getTextureID(std::string name)
	{
		return mTextures[name]->getID();
	}

	const unsigned int getTextureWidth(std::string name)
	{
		return mTextures[name]->getWidth();
	}

	const unsigned int getTextureHeight(std::string name)
	{
		return mTextures[name]->getHeight();
	}
	
	void clear();
};
