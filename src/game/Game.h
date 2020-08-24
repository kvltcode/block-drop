#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "GameObject.h"

#include "Board.h"

#include "PieceFactory.h"
#include "Piece.h"


#include "PieceColours.h"

#include "Config.h"

#include "Light.h"

#include <iostream>

#include <vector>
#include <map>
#include <tuple>

class Game
{
public:
	enum class GameState
	{
		START,
		ACTIVE,
		GAME_OVER
	};

	enum class Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

private:
	GLFWwindow* mWindow { nullptr };

	float mDeltaTime { 0.0f };
	float mLastFrameTime { 0.0f };	

	glm::vec4 mClearColour { 0.35f, 0.35f, 0.75f, 1.0f };

	ResourceManager mResourceManager;

	GameState mCurrentGameState { GameState::START };

	std::unique_ptr<Camera> mCamera;
		
	//mouse stuff
	double mMouseLastX { Config::screenWidth / 2.0 };
	double mMouseLastY { Config::screenHeight / 2.0 };
	bool mFirstMouseMove { true };

	//game pieces
	const PieceColours mPieceColours;

	Board mBoard { 10, 20 };
	PieceFactory mPieceFactory;
	Piece mCurrentPiece; //todo - Change to an array to handle preview piece
	Piece mGhostPiece;
	
	//other game objects
	Light mLight { glm::vec3(250.0f, 0.0f, 0.0f) };
	std::unique_ptr<GameObject> mDepthMapRender;
	
	//defaults
	float mDropTimer { 0 };
	const float cDropSlowest { 1000 };
	const float cDropFastest { 50 };
	const float cDropRate { 50 };

	int mScore { 0 };
	int mLines { 0 };
	int mLevel { 0 };

	void setPosition();
	void updateScore(int noRowsCleared);
	void updateGhostPosition();

	void restartGame(bool initPiece = true);
    void setGameState(GameState state);

	//main game objects
	void drawGameObjects(std::unique_ptr<Shader>& shader, bool renderDepth = true);

	//text in 2D projection
	void drawHUD();
	void drawIntro();
	void drawGameOver();

	//imgui stuff	
	const ImVec4 mImGuiHighlightText {0.8f, 0.4f, 0.8f, 1.0f};
	void drawDebug();

	//testing
	bool mFreeMoveMode = false;
	bool mDrawlight = true;
	bool mDrawDepthMap = true;
	bool mDebugMenu = true;
	bool mDrawCurrentPiece = true;
	bool mDrawGhostPiece = true;
	bool mDrawBoardBorder = true;
	bool mDrawBoard = true;
	bool mRunDrawTest = false;	
	
	float anchorX = 0.0f;
	float anchorY = 0.0f;
	void drawTest();

public:
	Game();
	~Game();

	Game(const Game&) 				= delete; //no copy constructor
	Game& operator=(const Game&) 	= delete; //no copy assignment
	Game(Game&&) 					= delete; //no move constructor
	Game& operator=(Game&&) 		= delete; //no move assignment
	
	bool poll();
	void update();
	void draw();

	//////////////////////
	// callbacks
	//////////////////////
	void resizeWindow(int width, int height)
	{
		Config::screenWidth = width;
		Config::screenHeight = height;
		glViewport(0, 0, Config::screenWidth, Config::screenHeight);
	}

	void getMousePosition(double xPos, double yPos)
	{
		if (mFirstMouseMove)
		{
			mMouseLastX = xPos;
			mMouseLastY = yPos;
			mFirstMouseMove = false;
		}

		double xOffset = xPos - mMouseLastX;
		double yOffset = mMouseLastY - yPos; // reversed since y-coordinates go from bottom to top
		mMouseLastX = xPos;
		mMouseLastY = yPos;

		if(!mDebugMenu)
		{
			mCamera->handleMouseMovement(xOffset, yOffset);
		}
	}
};

//////////////////////
// callbacks
//////////////////////
static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//need to get the user pointer first
	Game* temp = static_cast<Game*>(glfwGetWindowUserPointer(window));
	std::cout << "Resizing\n";
	temp->resizeWindow(width, height); 	
}

static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	Game* temp = static_cast<Game*>(glfwGetWindowUserPointer(window));
	temp->getMousePosition(xPos, yPos);
}

static int sLastKey;
static int sLastKeyAction;
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	sLastKeyAction = action;
	sLastKey = key;
}