cmake_minimum_required(VERSION 3.8)

project(bnahabra-renderer VERSION 1.0 LANGUAGES CXX)

add_subdirectory(imgui)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra" CACHE STRING "C++ compiler flags" FORCE)

set(RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/res)
set(SHADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src/window.cc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src/pipeline.cc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh.cc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src/object.cc)

target_compile_definitions(${PROJECT_NAME} PUBLIC RESOURCES_PATH="${RESOURCES_PATH}/")
target_compile_definitions(${PROJECT_NAME} PUBLIC SHADERS_PATH="${SHADERS_PATH}/")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE Imgui::Imgui glfw OpenGL::GL GLEW::GLEW glm::glm)