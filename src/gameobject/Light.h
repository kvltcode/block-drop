#pragma once

#include "Config.h"
#include "GameObject.h"

class Light: public GameObject
{
private:	

	glm::vec3 	mAmbientColour 		{ 0.1f, 0.1f, 0.1f };
	glm::vec3 	mDiffuseColour		{ 0.4f, 0.4f, 0.4f };
	float 		mSpecularShininess 	{ 1024.0f }; 

	float 		mNearClip 			{ 1.0f };
	float 		mFarClip 			{ 5000.0f };

	glm::mat4   mProjection			{ 0.0f };
	
public:
	Light(glm::vec3 position);

	glm::vec3& 	getAmbientColour() { return mAmbientColour; }
	glm::vec3& 	getDiffuseColour() { return mDiffuseColour; }
	float& 		getSpecularShininess() { return mSpecularShininess; }
	
	const float getNearClip() const { return mNearClip; }
	const float getFarClip() const { return mFarClip; }

	const glm::mat4 getLightSpaceMatrix() const;
};