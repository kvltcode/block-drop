#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class GameObject
{
protected:	
	glm::mat4 mModel 	{ glm::mat4{1.0f} };

	glm::vec3 mPosition {0.0f, 0.0f, 0.0f};
	glm::vec3 mScale 	{1.0f, 1.0f, 1.0f};
	glm::vec3 mRotation {0.0f, 0.0f, 0.0f};

	glm::vec4 mColour  	{1.0f, 1.0f, 1.0f, 1.0f};

	void updateModel();

public:
	GameObject() {};
	GameObject(glm::vec3 position);

	const glm::mat4& getModel() const;

	const glm::vec4& getColour() const { return mColour; }
	glm::vec4& getColour() { return mColour; }

	const glm::vec3& getPosition() const { return mPosition; }
	const float getPositionX() const { return mPosition.x; }
	const float getPositionY() const { return mPosition.y; }
	const float getPositionZ() const { return mPosition.z; }

	const glm::vec3& getScale() const { return mScale; }

	void setPosition(glm::vec3 position);
	void setPosition(float x, float y, float z = 0.0f);
	void setPositionX(float position);
	void setPositionY(float position);
	void setPositionZ(float position);

	void setScale(glm::vec3 scale);
	void setScale(float x, float y, float z = 1.0f);

	void setRotation(glm::vec3 rotation);
	void setRotation(float x, float y, float z = 0.0f);
	void setRotationX(float rotation);
	void setRotationY(float rotation);
	void setRotationZ(float rotation);

	void setColour(glm::vec4 colour) { mColour = colour; }
	void setColour(float r, float g, float b, float a = 1.0f);
};

