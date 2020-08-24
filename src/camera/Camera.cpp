#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight, glm::vec3 initialPosition)
	:mScreenWidth(screenWidth), mScreenHeight(screenHeight), mInitialCameraPos(initialPosition)
{
	reset();	
}

void Camera::reset()
{
	mCurrentCameraPos = mInitialCameraPos;

	mCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	mFieldOfView = cOriginalFieldOfView;
	mNearClip = cOriginalNearClip;
	mFarClip = cOriginalFarClip;

	mYaw = cOriginalYaw;	
	mPitch = cOriginalPitch;

	updateLookAt();
	updateProjection();
}

void Camera::updateLookAt()
{
	mView = glm::lookAt(mCurrentCameraPos,
		mCurrentCameraPos + mCameraFront, //direction
		mCameraUp);
}

void Camera::updateProjection(float width, float height)
{
	mScreenWidth = width;
	mScreenHeight = height;

	if (mScreenWidth <= 0) { mScreenWidth = 1; }
	if (mScreenHeight <= 0) { mScreenHeight = 1; }

	updateProjection();
}

void Camera::updateProjection()
{
	//update both 3D and 2D projections
	mProjection = glm::perspective(glm::radians(mFieldOfView), mScreenWidth / mScreenHeight, mNearClip, mFarClip);
	mProjection2D = glm::ortho(0.0f, mScreenWidth, 0.0f, mScreenHeight, -1.0f, 1.0f);
}

void Camera::handleKeyboardInput(CameraMovement type, float deltaTime)
{
	glm::vec3 cameraRight{ 0.0f };
	switch(type)
	{		
		case CameraMovement::Forward:
			mCurrentCameraPos += mMovementSpeed * mCameraFront * deltaTime;
		break;
		case CameraMovement::Back:
			mCurrentCameraPos -= mMovementSpeed * mCameraFront * deltaTime;
		break;
		case CameraMovement::StrafeLeft:
			cameraRight = glm::normalize(glm::cross(mCameraFront, mCameraUp));
			mCurrentCameraPos -= cameraRight * mStrafeSpeed * deltaTime;
		break;
		case CameraMovement::StrafeRight:
			cameraRight = glm::normalize(glm::cross(mCameraFront, mCameraUp));
			mCurrentCameraPos += cameraRight * mStrafeSpeed * deltaTime;
		break;
		case CameraMovement::Up:
			mCurrentCameraPos += mMovementSpeed * mCameraUp * deltaTime;
		break;
		case CameraMovement::Down:
			mCurrentCameraPos -= mMovementSpeed * mCameraUp * deltaTime;
		break;
	}

	updateLookAt();
}

//constain pitch so we don't flip out
void Camera::handleMouseMovement(double xOffset, double yOffset, bool constrainPitch)
{
	xOffset *= mMouseSensitivity;
	yOffset *= mMouseSensitivity;

	mYaw += static_cast<float>(xOffset);
	mPitch += static_cast<float>(yOffset);

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (mPitch > 89.0f)
	{
		mPitch = 89.0f;
	}
	if (mPitch < -89.0f)
	{
		mPitch = -89.0f;
	}

	glm::vec3 tempFront;
	tempFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	tempFront.y = sin(glm::radians(mPitch));
	tempFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mCameraFront = glm::normalize(tempFront);

	updateLookAt();
}

void Camera::handleMouseScroll(double yOffset)
{
	if (mFieldOfView >= 1.0f && mFieldOfView <= 45.0f)
	{
		mFieldOfView -= static_cast<float>(yOffset);
	}
	else if (mFieldOfView <= 1.0f)
	{
		mFieldOfView = 1.0f;
	}
	else if (mFieldOfView >= 45.0f)
	{
		mFieldOfView = 45.0f;
	}

	updateProjection();
}
