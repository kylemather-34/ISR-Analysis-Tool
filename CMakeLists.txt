cmake_minimum_required(VERSION 3.20)

project(ISRAnalysisTool VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect sources
file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(ISRAnalysisTool ${SOURCES})

# Copy controls.json into build dir
configure_file(
    ${CMAKE_SOURCE_DIR}/data/controls.json
    ${CMAKE_BINARY_DIR}/data/controls.json
    COPYONLY
)
