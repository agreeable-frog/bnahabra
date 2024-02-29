cmake_minimum_required(VERSION 3.7)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
endif()

project(bnahabra VERSION 1.0 LANGUAGES CXX)

add_subdirectory(renderer)