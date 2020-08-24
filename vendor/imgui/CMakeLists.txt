project(imgui)

add_library(${PROJECT_NAME} 
imconfig.h
imgui.cpp
imgui.h
imgui_demo.cpp
imgui_draw.cpp
imgui_impl_glfw.cpp
imgui_impl_glfw.h
imgui_impl_opengl3.cpp
imgui_impl_opengl3.h
imgui_internal.h
imgui_widgets.cpp
imstb_textedit.h
imstb_rectpack.h
imstb_truetype.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glad)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glad/include)