cmake_minimum_required(VERSION 3.1)

project(libw3d CXX)

set(LIBW3D_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)

option(LIBW3D_BUILD_TOOLS "Build tools that use libw3d" ON)
if(LIBW3D_BUILD_TOOLS)
	add_subdirectory(tools)
endif()