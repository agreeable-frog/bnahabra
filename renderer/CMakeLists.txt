cmake_minimum_required(VERSION 3.7)

project(renderer VERSION 1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message( STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

message( STATUS "${PROJECT_NAME} Build type is : ${CMAKE_BUILD_TYPE}")

add_subdirectory(imgui)
add_subdirectory(stb)
add_subdirectory(OBJ_loader)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SHADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

add_library(${PROJECT_NAME} STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/window.cc
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/pipeline.cc
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh.cc
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cc
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/object.cc
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/texture2D.cc
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/texture_atlas.cc
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/obj_loader.cc
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/framebuffer.cc)
add_library(bnahabra::renderer ALIAS ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PUBLIC SHADERS_PATH="${SHADERS_PATH}/")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(TARGET glm::glm)
set(${GLM_LIB} glm::glm)
else()
message( STATUS "Did not find imported target for glm")
set(${GLM_LIB} glm)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC Imgui::Imgui stb::stb glfw ${CMAKE_DL_LIBS} ${OPENGL_LIBRARIES} GLEW::GLEW ${GLM_LIB} obj-loader::obj-loader)