cmake_minimum_required(VERSION 3.7)

project(bnahabra-renderer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/res)
set(SHADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
                               # Imgui files
                               ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
                               ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
                               ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
                               ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_impl_glfw.cpp
                               ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_impl_opengl3.cpp
                               ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
                               ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp)

target_compile_definitions(${PROJECT_NAME} PUBLIC RESOURCES_PATH="${RESOURCES_PATH}/")
target_compile_definitions(${PROJECT_NAME} PUBLIC SHADERS_PATH="${SHADERS_PATH}/")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::OpenGL GLEW::GLEW ${glfw} glm::glm)