project(BlockDrop)

set(CAMERA 
    "${CMAKE_CURRENT_SOURCE_DIR}/camera/Camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/camera/Camera.h"   
)

set(CONFIG 
    "${CMAKE_CURRENT_SOURCE_DIR}/config/config.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/config/config.cpp"
)

set(DRAWABLE 
    "${CMAKE_CURRENT_SOURCE_DIR}/drawable/Quad.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/drawable/Quad.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/drawable/TexturedQuad.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/drawable/TexturedQuad.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/drawable/Cube.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/drawable/Cube.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/drawable/TexturedCube.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/drawable/TexturedCube.h"
)

set(GAME 
    "${CMAKE_CURRENT_SOURCE_DIR}/game/Game.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/game/Game.h"   
)

set(GAMEOBJECT
    "${CMAKE_CURRENT_SOURCE_DIR}/gameobject/GameObject.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/gameobject/GameObject.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/gameobject/Board.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/gameobject/Board.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/gameobject/Light.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/gameobject/Light.h"
)

set(GL_ELEMENTS 
    "${CMAKE_CURRENT_SOURCE_DIR}/glelements/IndexBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/glelements/IndexBuffer.h"    
    "${CMAKE_CURRENT_SOURCE_DIR}/glelements/VertexArrayObject.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/glelements/VertexArrayObject.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/glelements/VertexBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/glelements/VertexBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/glelements/VertexBufferLayout.h"    

    "${CMAKE_CURRENT_SOURCE_DIR}/glelements/DepthBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/glelements/DepthBuffer.cpp"   
)

set(PIECE
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/Cell.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/IPiece.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/PieceColours.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/PieceFactory.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/PieceFactory.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/Piece.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/Piece.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/piece/PieceI.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/PieceO.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/PieceT.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/PieceS.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/PieceZ.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/PieceJ.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/piece/PieceL.h"
)

set(RENDERER 
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/Renderer.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/Renderer3D.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/Renderer3D.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/RenderStats.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/RenderStats.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/TextRenderer.cpp"   
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/TextRenderer.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/QuadVertex.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/ModelVertex.h"
)

set(RESOURCE_MANAGER 
    "${CMAKE_CURRENT_SOURCE_DIR}/resourcemanager/ResourceManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/resourcemanager/ResourceManager.h"   
)

set(SHADER 
    "${CMAKE_CURRENT_SOURCE_DIR}/shader/Shader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/shader/Shader.h" 
)

set(TEXTURE 
    "${CMAKE_CURRENT_SOURCE_DIR}/texture/CubeMapTexture.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/texture/CubeMapTexture.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/texture/Texture.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/texture/Texture.h"   
)

set(UTILITY 
    "${CMAKE_CURRENT_SOURCE_DIR}/utility/Random.h"
)

add_executable(${PROJECT_NAME}  main.cpp
                                ${CAMERA}
                                ${CONFIG}
                                ${DRAWABLE}
                                ${GAME}
                                ${GAMEOBJECT}
                                ${GL_ELEMENTS} 
                                ${PIECE}                                                                
                                ${RESOURCE_MANAGER}
                                ${RENDERER}                               
                                ${SHADER}
                                ${TEXTURE}
                                ${UTILITY}
                                )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} stb_image)
target_link_libraries(${PROJECT_NAME} freetype)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glm)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/stb_image)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/freetype/include)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/camera)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/config)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/drawable)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/renderer)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/game)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gameobject)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glelements)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/piece)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/resourcemanager)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/shader)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/text)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/texture)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utility)

#copy the resource folder so the paths are still lined up
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/res/
                $<TARGET_FILE_DIR:BlockDrop>/res
                COMMENT "Moving images to target"
                VERBATIM)