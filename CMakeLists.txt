cmake_minimum_required(VERSION 3.7)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

message("Build type is : ${CMAKE_BUILD_TYPE}")

project(bnahabra VERSION 1.0 LANGUAGES CXX)

add_subdirectory(renderer)