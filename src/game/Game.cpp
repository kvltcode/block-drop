#include "Game.h"

#include "RenderStats.h"
#include "TextRenderer.h"
#include "Renderer3D.h"

#include <iostream>

Game::Game()
{
    if (!glfwInit())
    {
        std::cout << "Failed to init GLFW\n";
        std::terminate();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4); //v3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    mWindow = glfwCreateWindow(Config::screenWidth, Config::screenHeight, "Block Drop", NULL, NULL);
    if (!mWindow)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        std::terminate();
    }

    //Make the window's context current 
    glfwMakeContextCurrent(mWindow);

    //set callbacks
    glfwSetWindowUserPointer(mWindow, this); //handle for callbacks
    glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);
    glfwSetKeyCallback(mWindow, keyCallback);
    glfwSetWindowSizeLimits(mWindow, Config::screenWidth, Config::screenHeight, Config::screenWidth, Config::screenHeight); //lock out resizing
    glfwSetWindowPos(mWindow, 100, 100);
    glfwSetCursorPosCallback(mWindow, cursorPositionCallback);

    //syncs v sync
    //glfwSwapInterval(1);    

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    //if(!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        std::terminate();
    }

    //camera
    mCamera = std::make_unique<Camera>(Config::screenWidth, Config::screenHeight, glm::vec3{ 250.0f,-570.0f, 2000.0f });

    //load up resources
    mResourceManager.loadShader("ambient", "res/shaders/ambient.shader"); //plain shader with ambient lighting
    mResourceManager.loadShader("depth-map-visual", "res/shaders/depth-map-visual.shader"); //debugging shader for rendering the depth map to a quad
    mResourceManager.loadShader("lit-shadow-textured", "res/shaders/lit-shadow-textured.shader"); //blinn-phong lighting, showmapping and gamma correction
    mResourceManager.loadShader("post-process", "res/shaders/post-process.shader"); //post processing - not used. Need to implement extra buffer set up for it    
    mResourceManager.loadShader("shadowmap", "res/shaders/shadowmap.shader"); //base depth map render for shadow map
    mResourceManager.loadShader("skybox", "res/shaders/skybox.shader"); //skybox
    mResourceManager.loadShader("text", "res/shaders/text.shader"); //text
    mResourceManager.loadShader("test-model", "res/shaders/test-model.shader"); //test

    
    mResourceManager.loadTexture("default", "res/textures/default.png"); //small 16x16px texture for drawing plain coloured objects
    mResourceManager.loadTexture("piece", "res/textures/piece.png");
    mResourceManager.loadTexture("ghost", "res/textures/ghost.png");

    std::cout << "Default: " << mResourceManager.getTextureID("default") << "\n";
    std::cout << "Piece: " << mResourceManager.getTextureID("piece") << "\n";
    std::cout << "Ghost: " << mResourceManager.getTextureID("ghost") << "\n";

    mResourceManager.loadCubeMapTexture("skybox", 
        {
            "res/textures/sb-right.png",
            "res/textures/sb-left.png",
            "res/textures/sb-top.png",
            "res/textures/sb-bottom.png",
            "res/textures/sb-front.png",
            "res/textures/sb-back.png"
        }        
    );
   
    //init the renderer
    Renderer::GetInstance().init(Config::screenWidth, Config::screenHeight);
   
    //text
    TextRenderer::init();
    TextRenderer::loadFontFace("Default", "res/fonts/Roboto-Medium.ttf"); //fix this, needs another texture
    TextRenderer::loadFontFace("Retro", "res/fonts/PressStart2P.ttf"); //switched fonts round

    Renderer3D::init();

    //objects
    mDepthMapRender = std::make_unique<GameObject>(glm::vec3{0.0f, 0.0f, 0.0f});
    mDepthMapRender->setRotationZ(glm::radians(180.0f)); //flip it 180 to render correctly
    mDepthMapRender->setScale(480.0f, 270.0f, 1.0f);
    mDepthMapRender->setPosition(Config::screenWidth - 240.0f, Config::screenHeight - 135.0f, 0.0f);  

    //imgui stuff
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    const char* glsl_version = "#version 130";
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load IMGUI Fonts
    io.Fonts->AddFontFromFileTTF("res/fonts/Roboto-Medium.ttf", 16.0f);

    // OpenGL configuration
    glViewport(0, 0, Config::screenWidth, Config::screenHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_MULTISAMPLE);
    mDebugMenu ? glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL) : glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   

    //shader configuration
    //set constants that won't change during run
    {
        auto& shader = mResourceManager.getShader("depth-map-visual");
        shader->bind();    
        shader->setUniform1i("u_Texture", 0);
    }

    {
        auto& shader = mResourceManager.getShader("lit-shadow-textured");
        shader->bind();    
        shader->setUniform1i("diffuseTexture", 0);
        shader->setUniform1i("shadowMap", 1);
    }

    {
        auto& shader = mResourceManager.getShader("skybox");
        shader->bind();    
        shader->setUniform1i("u_Texture", 0);
    }  

    //setup game
    restartGame(false);

}

Game::~Game()
{
    TextRenderer::shutdown();
    Renderer3D::shutdown();
    mResourceManager.clear();
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void Game::setPosition()
{
    std::cout << "Setting position\n";
                
    //lock position
    mCurrentPiece.transferPieceToBoard(mBoard);
    
    //check for complete rows
    int noRowsCleared = mBoard.checkForCompletedRows();
    if(noRowsCleared > 0)
    {
        updateScore(noRowsCleared);
    }
    
    //generate new piece
    mCurrentPiece = mCurrentPiece = mPieceFactory.getNewPiece();

    //copy to ghost
    updateGhostPosition();

    //check for game over
    if(mCurrentPiece.isCollision(mBoard))
    {
        std::cout << "GAMEOVER!!!\n";
        mCurrentGameState = GameState::GAME_OVER;
    };
}

void Game::updateScore(int noRowsCleared)
{
    std::cout << "Number of completed rows: " << noRowsCleared << "\n";    

    mLines += noRowsCleared;

    switch(noRowsCleared)
    {
        case 1:
            mScore += (40 * (mLevel + 1));
        break;            
        case 2:
            mScore += (100 * (mLevel + 1));	
        break;
        case 3:
            mScore += (300 * (mLevel + 1));
        break;
        case 4:
            mScore += (1200 * (mLevel + 1));
        break;
    }
    
    //update level
    mLevel = mLines / 10;
}

void Game::updateGhostPosition()
{
    mGhostPiece = mCurrentPiece; //just replace the old one with a new piece
    while (mGhostPiece.moveDown(mBoard));
}

void Game::restartGame(bool initPiece /*= true*/)
{
    //clear board
    mBoard.reset();
    //clear score
    mScore = 0;    
    //clear lines
    mLines = 0;
    //clear level
    mLevel = 0;
    //reset timer
    mDropTimer = 0;

    if(initPiece)
    {
        //set up new piece
        mCurrentPiece = mPieceFactory.getNewPiece();
        //set up ghost
        updateGhostPosition();
    }

    
}

void Game::setGameState(GameState state)
{
    if (mCurrentGameState != state)
    {
        mCurrentGameState = state;
    }

    if(mCurrentGameState == GameState::ACTIVE)
    {
        restartGame();
    }
}

bool Game::poll()
{
    glfwPollEvents();

    if (sLastKeyAction == GLFW_PRESS || sLastKeyAction == GLFW_REPEAT)
    {
        ////////////////////////////////////////////////////////////    
        //All screens
        ////////////////////////////////////////////////////////////
        if (sLastKey == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(mWindow, true);
        }

        if (sLastKey == GLFW_KEY_F1)
        {
            if(mDebugMenu)
            {
                mDebugMenu = false;
                glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);                
            }
            else
            {
                mDebugMenu = true;
                glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);                
            }

            sLastKeyAction = GLFW_RELEASE;            
        }        
        
        if (sLastKey == GLFW_KEY_Q)
        {
            mCamera->handleKeyboardInput(Camera::CameraMovement::Up, mDeltaTime);
        }

        if (sLastKey == GLFW_KEY_A)
        {
            mCamera->handleKeyboardInput(Camera::CameraMovement::StrafeLeft, mDeltaTime);
        }  

        if (sLastKey == GLFW_KEY_D)
        {
            mCamera->handleKeyboardInput(Camera::CameraMovement::StrafeRight, mDeltaTime);
        }        

        if (sLastKey == GLFW_KEY_E)
        {
            mCamera->handleKeyboardInput(Camera::CameraMovement::Down, mDeltaTime);
        }

         if (sLastKey == GLFW_KEY_W)
        {
            mCamera->handleKeyboardInput(Camera::CameraMovement::Forward, mDeltaTime);
        }

        if (sLastKey == GLFW_KEY_S)
        {
            mCamera->handleKeyboardInput(Camera::CameraMovement::Back, mDeltaTime);
        }
    
        if (sLastKey == GLFW_KEY_H)
        {
            std::cout << "Reset camera\n";
            mCamera->reset();
            sLastKeyAction = GLFW_RELEASE;
        }
        
        ////////////////////////////////////////////////////////////    
        //Screens
        ////////////////////////////////////////////////////////////
        switch (mCurrentGameState)
        {
            case GameState::START:
            {
                if (sLastKey == GLFW_KEY_SPACE)
                {
                    //reset / set up game
                    setGameState(GameState::ACTIVE);
                    sLastKeyAction = GLFW_RELEASE;
                }
            }
            break;
            case GameState::ACTIVE:
            {
                //up
                if (sLastKey == GLFW_KEY_UP)
                {
                    if(mFreeMoveMode)
                    {
                        mCurrentPiece.moveUp(mBoard);
                        updateGhostPosition(); //not technically needed        
                        sLastKeyAction = GLFW_RELEASE;
                    }
                }

                //down
                if (sLastKey == GLFW_KEY_DOWN)
                {
                    bool result = mCurrentPiece.moveDown(mBoard);
                    
                    sLastKeyAction = GLFW_RELEASE;

                    if(mFreeMoveMode)
                    {
                        return true;
                    }

                    if(result)
                    {
                        ++mScore;
                    }
                    else            
                    {   
                        setPosition();
                    }                   
                }

                //hard drop
                if (sLastKey == GLFW_KEY_SPACE)
                {      
                    bool okToMove = true;
                    while (okToMove)
                    {                
                        okToMove = mCurrentPiece.moveDown(mBoard);
                        if(okToMove && !mFreeMoveMode) 
                        {
                            ++mScore;
                        }
                    }           
                                
                    setPosition();           
                    
                    sLastKeyAction = GLFW_RELEASE;
                }

                //left
                if (sLastKey == GLFW_KEY_LEFT)
                {   
                    mCurrentPiece.moveLeft(mBoard); 
                    updateGhostPosition();       
                    sLastKeyAction = GLFW_RELEASE;
                }

                //right
                if (sLastKey == GLFW_KEY_RIGHT)
                {            
                    mCurrentPiece.moveRight(mBoard);
                    updateGhostPosition();
                    sLastKeyAction = GLFW_RELEASE;
                }

                //clockwise
                if (sLastKey == GLFW_KEY_KP_0 || sLastKey == GLFW_KEY_X)
                {            
                    mCurrentPiece.rotateClockwise(mBoard);
                    updateGhostPosition();
                    sLastKeyAction = GLFW_RELEASE;
                }

                //anticlockwise
                if (sLastKey == GLFW_KEY_RIGHT_CONTROL || sLastKey == GLFW_KEY_Z)
                {   
                    mCurrentPiece.rotateCounterClockwise(mBoard);         
                    updateGhostPosition();
                    sLastKeyAction = GLFW_RELEASE;
                }
            }
            break;
            case GameState::GAME_OVER:
            {
                if (sLastKey == GLFW_KEY_SPACE)
                {
                    //reset / set up game
                    setGameState(GameState::ACTIVE);
                    sLastKeyAction = GLFW_RELEASE;
                }
            }
        break;
        }
    }

    if (glfwWindowShouldClose(mWindow))
    {
        return false;
    }

    return true;
}

void Game::update()
{
    float currentFrame = glfwGetTime();
    mDeltaTime = currentFrame - mLastFrameTime;
    mLastFrameTime = currentFrame;
   
    if(mCurrentGameState == GameState::ACTIVE && !mFreeMoveMode)
    {
        mDropTimer += (mDeltaTime * 1000); //get it to millisecs so it's a bit easier to read

        float drapCap = cDropSlowest - (cDropRate* mLevel);
        if (drapCap <= cDropFastest)
        {
           drapCap = cDropFastest;
        }

        if (mDropTimer >= drapCap)
        {
            if(!mCurrentPiece.moveDown(mBoard))                    
            {   
                setPosition();
            }
            mDropTimer -= drapCap;
            if (mDropTimer <= 0)
            {
                mDropTimer = 0;
            }
        }
    }
}

void Game::drawHUD()
{
    TextRenderer::setFont("Default");
    TextRenderer::setFontColour(glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));

    TextRenderer::begin(mResourceManager.getShader("text"), mCamera->getProjectionView());
    
        float xPos = 700.0f;
        float yPos = -50.0f;
        float yGap = 60.0f;
        TextRenderer::renderText("Score: " + std::to_string(mScore), glm::vec2(xPos, yPos), 1.25f);
        yPos -= yGap;
        TextRenderer::renderText("Lines: " + std::to_string(mLines), glm::vec2(xPos, yPos), 1.25f);
        yPos -= yGap;
        TextRenderer::renderText("Level: " + std::to_string(mLevel), glm::vec2(xPos, yPos), 1.25f);
    
    TextRenderer::end(); 
}

//this stuff is slow due to the number of draw calls
//would have been more efficient doing it as a texture
void Game::drawIntro()
{
    TextRenderer::setFont("Retro");
    TextRenderer::setFontColour(glm::vec4(0.97f, 0.97f, 0.97f, 1.0f));

    TextRenderer::begin(mResourceManager.getShader("text"), mCamera->getProjectionView2D());

        float xPos = Config::screenWidth / 2.0f;
        float yPos = Config::screenHeight - 300.0f;

        std::string stringToRender = "BLOCK DROP!";
        float stringToRenderWidth = TextRenderer::getStringWidth(stringToRender, 1.6f);
        TextRenderer::renderText(stringToRender, glm::vec2(xPos - (stringToRenderWidth / 2), yPos), 1.6f);
        yPos -= 250.0f;

        stringToRender = "Press SPACE to start!";
        stringToRenderWidth = TextRenderer::getStringWidth(stringToRender, 1.0f);
        TextRenderer::renderText(stringToRender, glm::vec2(xPos - (stringToRenderWidth / 2), yPos), 1.0f);

        //controls
        //main game
        const float increment = 30.0f;
        xPos = 100.0f;
        yPos = 280.0f;
    
        std::string mainGameControls[]
        {
            "Left arrow - Move block left",
            "Right arrow - Move block right",
            "Down arrow - Move block down",
            "Keypad 0 or X - Rotate block clockwise",
            "Right CTRL or Z - Rotate block anticlockwise",
            "Space - Hard drop"
        };

        TextRenderer::renderText("Main Game Controls", glm::vec2(xPos, yPos), 0.6f);
        yPos -= increment;
        for (auto& text : mainGameControls)
        {
            TextRenderer::renderText(text, glm::vec2(xPos, yPos), 0.4f);
            yPos -= increment;
        }

        //camera and debug
        std::string debugControls[]
        {
            "F1 - Toggle debug menu",
            "W - Move forward",
            "S - Move back",
            "A - Move left",
            "D - Move right",
            "Mouse - Look around",
            "H - Reset view",
            "Escape -  Exit"
        };

        xPos = 1200.0f;
        yPos = 280.0f;
        TextRenderer::renderText("Debug & camera controls", glm::vec2(xPos, yPos), 0.6f);
        yPos -= increment;
        for (auto& text : debugControls)
        {
            TextRenderer::renderText(text, glm::vec2(xPos, yPos), 0.4f);
            yPos -= increment;
        }   
    TextRenderer::end(); 
}

void Game::drawGameOver()
{
    TextRenderer::setFont("Retro");
    TextRenderer::setFontColour(glm::vec4(0.7f, 0.97f, 0.97f, 1.0f));

    TextRenderer::begin(mResourceManager.getShader("text"), mCamera->getProjectionView2D());
    
        float xPos = Config::screenWidth / 2.0f;
        float yPos = Config::screenHeight - 300.0f;    
        std::string stringToRender = "GAMEOVER!";
        float stringToRenderWidth = TextRenderer::getStringWidth(stringToRender, 1.6f);
        TextRenderer::renderText(stringToRender, glm::vec2(xPos - (stringToRenderWidth / 2), yPos), 1.6f);
        yPos -= 200.0f;

        stringToRender = "Score!";
        stringToRenderWidth = TextRenderer::getStringWidth(stringToRender, 1.0f);
        TextRenderer::renderText(stringToRender, glm::vec2(xPos - (stringToRenderWidth / 2), yPos), 1.0f);
        yPos -= 60.0f;

        stringToRender = std::to_string(mScore);
        stringToRenderWidth = TextRenderer::getStringWidth(stringToRender, 1.0f);
        TextRenderer::renderText(stringToRender, glm::vec2(xPos - (stringToRenderWidth / 2), yPos), 1.0f);
        yPos -= 100.0f;

        stringToRender = "Press SPACE to retry!";
        stringToRenderWidth = TextRenderer::getStringWidth(stringToRender, 1.0f);
        TextRenderer::renderText(stringToRender, glm::vec2(xPos - (stringToRenderWidth / 2), yPos), 1.0f);
    TextRenderer::end(); 
}

void Game::drawDebug()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    //window_flags |= ImGuiWindowFlags_NoScrollbar;
    //window_flags |= ImGuiWindowFlags_MenuBar;
    //window_flags |= ImGuiWindowFlags_NoMove;
    //window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    //window_flags |= ImGuiWindowFlags_NoNav;
    //window_flags |= ImGuiWindowFlags_NoBackground;
    //window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    //if (no_close)           p_open = NULL; // Don't pass our bool* to Begin
            
    ImGui::Begin("Debug", NULL, window_flags);

    ImGui::TextColored(mImGuiHighlightText, "FPS");ImGui::SameLine();ImGui::Text("%.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

    ImGui::TextColored(mImGuiHighlightText, "Camera position");ImGui::SameLine();ImGui::Text("%.2f, %.2f, %.2f", mCamera->getPosition().x, mCamera->getPosition().y, mCamera->getPosition().z);
    ImGui::TextColored(mImGuiHighlightText, "Lock at direction");ImGui::SameLine();ImGui::Text("%.2f, %.2f, %.2f", mCamera->getFront().x, mCamera->getFront().y, mCamera->getFront().z);

    ImGui::Separator();
    auto lightPosition = mLight.getPosition();
    ImGui::TextColored(mImGuiHighlightText, "Light position");ImGui::SameLine();ImGui::Text("%.2f, %.2f, %.2f", lightPosition.x, lightPosition.y, lightPosition.z);
    ImGui::SliderFloat("X", &lightPosition.x, -1000.0f, 1000.0f);
    ImGui::SliderFloat("Y", &lightPosition.y, -1000.0f, 1000.0f);
    ImGui::SliderFloat("Z", &lightPosition.z, -1000.0f, 1000.0f);
    mLight.setPosition(lightPosition);

    ImGui::Separator();
    auto& lightColour = mLight.getDiffuseColour();
    ImGui::TextColored(mImGuiHighlightText, "Light colour");ImGui::SameLine();ImGui::Text("%.2f, %.2f, %.2f", lightColour.r, lightColour.g, lightColour.b);
    ImGui::SliderFloat("R", &lightColour.r, 0.0f, 1.0f);
    ImGui::SliderFloat("G", &lightColour.g, 0.0f, 1.0f);
    ImGui::SliderFloat("B", &lightColour.b, 0.0f, 1.0f);
   
    //draw scene selector
    ImGui::Separator();
    ImGui::TextColored(mImGuiHighlightText, "Current scene");ImGui::SameLine();   
    const char* items[] = { "Start", "Active", "Game Over" };
    const char* item_current = items[static_cast<int>(mCurrentGameState)];
    
    if (ImGui::BeginCombo("", item_current))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (item_current == items[n]);
            if (ImGui::Selectable(items[n], is_selected))
            {
                item_current = items[n];
                setGameState(static_cast<GameState>(n));
            }
        }
        ImGui::EndCombo();
    }

    //draw scene selector
    ImGui::Separator();
    ImGui::TextColored(mImGuiHighlightText, "Current level");ImGui::SameLine();   
    ImGui::SliderInt("", &mLevel, 0, 25);

    ImGui::Separator();
    ImGui::Checkbox("Render light object", &mDrawlight);
    
    ImGui::Separator();
    ImGui::Checkbox("Render depth map", &mDrawDepthMap);

    ImGui::Separator();
    ImGui::Checkbox("Render current piece", &mDrawCurrentPiece);
    ImGui::Checkbox("Render ghost piece", &mDrawGhostPiece);
    ImGui::Checkbox("Render board border", &mDrawBoardBorder);
    ImGui::Checkbox("Render board", &mDrawBoard);    

    ImGui::Separator();
    ImGui::Checkbox("Piece free move", &mFreeMoveMode);    

    ImGui::Separator();
    ImGui::TextColored(mImGuiHighlightText, "Draw calls");ImGui::SameLine();ImGui::Text("%i", renderstats::drawCount);
    ImGui::TextColored(mImGuiHighlightText, "Quad count");ImGui::SameLine();ImGui::Text("%i", renderstats::quadCount);
    ImGui::TextColored(mImGuiHighlightText, "Cube count");ImGui::SameLine();ImGui::Text("%i", renderstats::cubeCount);
    ImGui::TextColored(mImGuiHighlightText, "Total Triangle count");ImGui::SameLine();ImGui::Text("%i", (renderstats::quadCount) * 2 + (renderstats::cubeCount * 12));
    ImGui::TextColored(mImGuiHighlightText, "Vertice count");ImGui::SameLine();ImGui::Text("%i", renderstats::verticeCount);
    ImGui::Separator();

    ImGui::Checkbox("Render test renderer", &mRunDrawTest);
    ImGui::SliderFloat("Anchor X", &anchorX, -1000.0f, 1000.0f);
    ImGui::SliderFloat("Anchor Y", &anchorY, -1000.0f, 1000.0f);

    ImGui::End();
        
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::drawTest()
{
   
    ///////////////////////text////////////////////////
    TextRenderer::begin(mResourceManager.getShader("text"), mCamera->getProjectionView2D());
    TextRenderer::setFont("Default");
    TextRenderer::setFontColour(glm::vec4(0.3, 0.212, 0.543, 0.5));
    
        TextRenderer::renderText("Testing", glm::vec2(0, 150), 2);
        TextRenderer::renderFullFontTexture(glm::vec2(0, 50));

    TextRenderer::end();

    ///////////////////////////////
    //test renderer stuff

    /////////////3d renderer /////////////////////////////////
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mResourceManager.getTextureID("default"));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mResourceManager.getTextureID("ghost"));  

    auto& shader = mResourceManager.getShader("test-model");
    shader->bind();
    shader->setUniformMat4f("u_Projection", mCamera->getProjection());
    shader->setUniformMat4f("u_View",  mCamera->getView());    

    int sampler[2] = {0, 1}; //active slots
    shader->setUniformArray1v("u_textures", 2, sampler);    

    Renderer3D::begin();        
    
        const int max = 100;
        const int size = 50;
        const int gap = 100;

        for (int y = 0; y < max; ++y)
        {
            for (int x = 0; x < max; ++x)
            {    
                Renderer3D::drawCube(glm::vec3(
                ((size + gap) * x) + anchorX,
                ((size + gap) * y) + anchorY - 1000,
                0.0f),
                glm::vec3(size, size, size),
                (x + y) % 2 == 0); //slot to use                
            }
        }

    Renderer3D::end();
}

//bind shader before passing in
void Game::drawGameObjects(std::unique_ptr<Shader>& shader, bool renderDepth)
{
    //currently dropping piece
    if (mDrawCurrentPiece)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mResourceManager.getTextureID("piece"));        

        auto& objects = mCurrentPiece.getCurrentPiece();

        for (int x = 0; x < objects.size(); ++x)
        {
            for (int y = 0; y < objects.at(x).size(); ++y)
            {         
                if (objects[x][y] != 0)
                {
                    shader->setUniformMat4f("u_Model", mCurrentPiece.getModel(x, y)); 
                    
                    auto colour = mPieceColours.colour.find(objects[x][y]);
                    if(colour != mPieceColours.colour.end())
                    {                        
                        shader->setUniformVec4f("u_Diffuse_Colour", colour->second); 
                    }

                    Renderer::GetInstance().drawTexturedCube();
                }
            }
        }
    }
   

    //game board border
    if (mDrawBoardBorder)
    {           
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mResourceManager.getTextureID("default"));        

        auto& objects = mBoard.getBorder();

        shader->setUniformVec4f("u_Diffuse_Colour", mBoard.getBoardColour(mLevel));   
        
        for (int x = 0; x < objects.size(); ++x)
        {
            shader->setUniformMat4f("u_Model", objects[x].getModel());
            Renderer::GetInstance().drawTexturedCube();                
        }
    }
    
    //game board
    if (mDrawBoard)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mResourceManager.getTextureID("piece"));        
               
        auto& objects = mBoard.getBoard();

        //render back to front order
        //for (int x = 0; x < objects.size(); ++x)
        for (int x = objects.size() - 1; x >= 0; --x)
        {
            //for (int y = 0; y < objects.at(x).size(); ++y)
            for (int y = objects.at(x).size() - 1; y >= 0; --y)
            {
                if(objects[x][y].type != 0)
                {
                    shader->setUniformMat4f("u_Model", objects[x][y].object.getModel());
                    
                    auto colour = mPieceColours.colour.find(objects[x][y].type);
                    if(colour != mPieceColours.colour.end())
                    {                        
                        shader->setUniformVec4f("u_Diffuse_Colour", colour->second); 
                    }

                    Renderer::GetInstance().drawTexturedCube();
                }               
            }
        }
    }
    
    //draw ghost piece
    if (!renderDepth && mDrawGhostPiece)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mResourceManager.getTextureID("ghost"));        
               
        auto& objects = mGhostPiece.getCurrentPiece();

        for (int x = 0; x < objects.size(); ++x)
        {
            for (int y = 0; y < objects.at(x).size(); ++y)
            {         
                if (objects[x][y] != 0)
                {
                    shader->setUniformMat4f("u_Model", mGhostPiece.getModel(x, y)); 
                    
                    auto colour = mPieceColours.colour.find(objects[x][y]);
                    if(colour != mPieceColours.colour.end())
                    {                        
                        shader->setUniformVec4f("u_Diffuse_Colour", colour->second); 
                    }
                    Renderer::GetInstance().drawTexturedCube();
                }
            }
        }
    }

   
}

void Game::draw()
{
    renderstats::resetStats();

    glClearColor(mClearColour.r, mClearColour.g, mClearColour.b, mClearColour.a);
    
    // 1 - Depth pass from the light's perspective
    Renderer::GetInstance().setupBuffer(RenderBuffer::DEPTH);

    auto& shader = mResourceManager.getShader("shadowmap");
    shader->bind();    
    shader->setUniformMat4f("u_LightSpaceMatrix", mLight.getLightSpaceMatrix());

    drawGameObjects(shader, true);

    //2 - Regular pass
    Renderer::GetInstance().setupBuffer();

    //render skybox
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / 200.0f, glm::vec3(0.0f, 1.0f, 0.0f)); //rotate around y axis 
            
        glDepthMask(GL_FALSE);
        //change depth function so depth test passes when values are equal to depth buffer's content
        glDepthFunc(GL_LEQUAL); 

        auto& shader = mResourceManager.getShader("skybox");
        shader->bind();
        shader->setUniformMat4f("u_Projection", mCamera->getProjection());
     
        glm::mat4 view = glm::mat4(glm::mat3(mCamera->getView()));  
        shader->setUniformMat4f("u_View", view);
        shader->setUniformMat4f("u_Model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mResourceManager.getCubeMapTextureID("skybox")); //maybe bind this in the manager?
            
        Renderer::GetInstance().drawTexturedCube();
            
        glDepthFunc(GL_LESS); // set depth function back to default
        glDepthMask(GL_TRUE);
    }

    //render main objects   
    {
        auto& shader = mResourceManager.getShader("lit-shadow-textured");
        shader->bind();

        glm::mat4 projection = mCamera->getProjection();
        glm::mat4 view = mCamera->getView();
        shader->setUniformMat4f("projection", projection);
        shader->setUniformMat4f("view", view);
        shader->setUniformVec3f("viewPos", mCamera->getPosition());

        shader->setUniformVec3f("lightPos", mLight.getPosition());
        shader->setUniformMat4f("u_LightSpaceMatrix", mLight.getLightSpaceMatrix());        
        shader->setUniformVec3f("u_Ambient_Colour", mLight.getAmbientColour());
        shader->setUniformVec3f("u_Light_Colour", mLight.getDiffuseColour());        
        shader->setUniform1f("u_Shininess", mLight.getSpecularShininess());

        //set the second texture to the depth texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Renderer::GetInstance().getBufferTexture(RenderBuffer::DEPTH));
                
        drawGameObjects(mResourceManager.getShader("lit-shadow-textured"), false);
    }

    //render light object (so you can see where it is)
    if (mDrawlight && mDebugMenu)
    {
        auto& shader = mResourceManager.getShader("ambient");
        shader->bind();
        shader->setUniformMat4f("u_Projection", mCamera->getProjection());
        shader->setUniformMat4f("u_View", mCamera->getView());
        shader->setUniformVec3f("u_DiffuseColour", mLight.getDiffuseColour());
        shader->setUniformVec3f("u_AmbientColour", mLight.getAmbientColour());
        
        shader->setUniformMat4f("u_Model", mLight.getModel());
        Renderer::GetInstance().drawCube();        
    }

    if(mCurrentGameState != GameState::START)
    {
        //render score etc
        drawHUD();
    }

    //render depth map
    if (mDrawDepthMap && mDebugMenu)
    {
        //2D render
        auto& shader = mResourceManager.getShader("depth-map-visual");
        shader->bind();

        shader->setUniformMat4f("u_Projection", mCamera->getProjection2D());
        shader->setUniformMat4f("u_View", glm::mat4(1.0f));    
        shader->setUniformMat4f("u_Model", mDepthMapRender->getModel());
        shader->setUniform1f("u_NearPlane", mLight.getNearClip()); 
        shader->setUniform1f("u_FarPlane", mLight.getFarClip());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Renderer::GetInstance().getBufferTexture(RenderBuffer::DEPTH));
            
        Renderer::GetInstance().drawQuad();
    }

    //2D projection stuff
    if(mCurrentGameState == GameState::START)
    {        
        drawIntro();
    }

    if(mCurrentGameState == GameState::GAME_OVER)
    {
        drawGameOver(); 
    }

    if(mRunDrawTest)
    {
        drawTest();
    }
 
    //draw IMGUI for testing output
    if(mDebugMenu)
    {
        drawDebug();    
    }

    //Swap front and back buffers
    glfwSwapBuffers(mWindow);
}