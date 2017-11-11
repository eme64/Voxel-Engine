cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

project(voxel)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW (http://www.glfw.org/docs/3.0/build.html)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glfw-3.2.1)
include_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glfw-3.2.1/include)

# GLEW
include_directories(GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/glew-2.1.0/include)
set(GLEW_LIBRARIES ${CMAKE_CURRENT_LIST_DIR}/thirdparty/glew-2.1.0/src/glew.c)

add_executable(voxel voxel.cpp ${GLEW_LIBRARIES})

target_link_libraries(voxel glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

add_definitions(-DMAIN_DIR="${CMAKE_CURRENT_LIST_DIR}/")
