cmake_minimum_required(VERSION 3.2)
project(wesh C CXX)
set(WESH_VERSION 0.2)

# directories variables
set(SRC_DIR src/)
set(OUT_DIR out/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/${OUT_DIR})

# sources dirs
add_subdirectory(${SRC_DIR})
