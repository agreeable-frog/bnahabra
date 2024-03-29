cmake_minimum_required(VERSION 3.8)

project(imgui VERSION 1.0 LANGUAGES CXX)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3 REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra" CACHE STRING "C++ compiler flags" FORCE)

add_library(${PROJECT_NAME} STATIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_glfw.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_opengl3.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp)
add_library(Imgui::Imgui ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)