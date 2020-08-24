#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{

public:
	enum class CameraMovement {
		Forward,
		Back,
		StrafeLeft,
		StrafeRight,
		Up,
		Down
	};

private:	

	//camera positions
	glm::vec3 mInitialCameraPos { 0.0f };
	glm::vec3 mCurrentCameraPos { 0.0f };

	const float cOriginalYaw { -90.0f }; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left
	const float cOriginalPitch { 0.0f };

	float mYaw { cOriginalYaw };
	float mPitch { cOriginalPitch };

	//speed
	float mMovementSpeed { 500.0f }; ///@todo Make this a factor of zoom position
	float mStrafeSpeed { 500.0f }; ///@todo Make this a factor of zoom position

	glm::vec3 mCameraUp { 0.0f };
	glm::vec3 mCameraFront { 0.0f };

	//projection
	const float cOriginalFieldOfView { 45.0f };
	const float cOriginalNearClip { 0.1f };
	const float cOriginalFarClip { 5000.0f };

	float mFieldOfView { cOriginalFieldOfView };
	float mNearClip { cOriginalNearClip };
	float mFarClip { cOriginalFarClip };

	float mScreenWidth;
	float mScreenHeight;

	//MVP
	glm::mat4 mProjection { 0.0f };
	glm::mat4 mView { 0.0f };

	//2D projection
	glm::mat4 mProjection2D { 0.0f};

	//mouse stuff
	float mMouseSensitivity { 0.1f };	

	void updateLookAt();
	void updateProjection();

public:
	Camera(float screenWidth, float screenHeight, glm::vec3 initialPosition);

	void reset();
	void updateProjection(float width, float height);

	void handleKeyboardInput(CameraMovement type, float deltaTime);
	void handleMouseMovement(double xOffset, double yOffset, bool constrainPitch = true);
	void handleMouseScroll(double yOffset);

	const glm::mat4& getProjection() const { return mProjection; };
	const glm::mat4& getProjection2D() const { return mProjection2D; };
	const glm::mat4& getView() const {return mView; };
	const glm::vec3& getPosition() const { return mCurrentCameraPos; };
	const glm::vec3& getFront() const { return mCameraFront; };

	const glm::mat4 getProjectionView() const { return mProjection * mView; };
	const glm::mat4 getProjectionView2D() const { return mProjection2D * glm::mat4(1.0f); };
};