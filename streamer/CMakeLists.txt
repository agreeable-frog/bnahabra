cmake_minimum_required(VERSION 3.7)

project(streamer VERSION 1.0 LANGUAGES CXX)

find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0)
pkg_check_modules(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0)
pkg_check_modules(gstreamer-rtsp-server REQUIRED IMPORTED_TARGET gstreamer-rtsp-server-1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_library(${PROJECT_NAME} 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/rtsp_pipeline.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/src/swapchain.cc)
add_library(bnahabra::streamer ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads PkgConfig::gstreamer PkgConfig::gstreamer-app PkgConfig::gstreamer-rtsp-server)