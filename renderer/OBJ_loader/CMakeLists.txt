cmake_minimum_required(VERSION 3.7)

project(obj-loader VERSION 1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

message("${PROJECT_NAME} Build type is : ${CMAKE_BUILD_TYPE}")

add_library(${PROJECT_NAME} INTERFACE)
add_library(obj-loader::obj-loader ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${PROJECT_NAME} INTERFACE TINYOBJLOADER_IMPLEMENTATION)
