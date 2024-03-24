cmake_minimum_required(VERSION 3.8)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

message("Build type is : ${CMAKE_BUILD_TYPE}")

project(bnahabra VERSION 1.0 LANGUAGES CXX)

add_subdirectory(renderer)
add_subdirectory(streamer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra" CACHE STRING "C++ compiler flags" FORCE)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE bnahabra::renderer bnahabra::streamer)